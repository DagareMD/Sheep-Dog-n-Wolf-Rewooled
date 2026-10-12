/* The SDL3 GPU backend of the renderer interface (src/render/render_device.h): Vulkan, Direct3D 12, or Metal,
 * through SDL_GPU, drawing into the game's own window.
 *
 * The draws of a frame are recorded, then replayed in as few render passes as possible: a clear, a texture changed
 * after the frame drew with it, or a frame copy ends one (Flush). Everything is submitted at Present.
 *
 * The window is the game's (src/platform/platform.h, made for this renderer: decorated, resizable, with Vulkan when that
 * is the driver); the device claims it for the GPU and leaves it to the platform layer afterwards. */
#include <SDL3/SDL.h>

#include "sdw_types.h"
#include "sdw_enums.h"
#include "../render_device.h"
#include "../perf_overlay.h"
#include "../../platform/platform.h"
#include "../../platform/options.h"
#include "sdl3_shaders.h"

/* d3dapp.cpp */
long D3DApp_StartWithoutDirectDraw(D3DApp *app, u32 width, u32 height);

#define SDL3_E_FAIL ((long)0x80004005L)
/* the render size: the options' (--width / --height, src/platform/options.h), 1920 x 1080 by default */
#define SDL3_RENDER_WIDTH (g_options.width > 0 ? g_options.width : 1920)
#define SDL3_RENDER_HEIGHT (g_options.height > 0 ? g_options.height : 1080)

/* Direct3D 7 values the backend interprets (render_device.h passes the game's) */
enum {
    D3D_BLEND_ZERO = 1,
    D3D_BLEND_ONE = 2,
    D3D_BLEND_SRCCOLOR = 3,
    D3D_BLEND_INVSRCCOLOR = 4,
    D3D_BLEND_SRCALPHA = 5,
    D3D_BLEND_INVSRCALPHA = 6,
    D3D_BLEND_DESTALPHA = 7,
    D3D_BLEND_INVDESTALPHA = 8,
    D3D_BLEND_DESTCOLOR = 9,
    D3D_BLEND_INVDESTCOLOR = 10,
    D3D_BLEND_SRCALPHASAT = 11,
    D3D_CULL_NONE = 1,
    D3D_CULL_CW = 2,
    D3D_CULL_CCW = 3,
    D3D_FVF_POSITION_MASK = 0x00e,
    D3D_FVF_NORMAL = 0x010,
    D3D_FVF_PSIZE = 0x020,
    D3D_FVF_TEXCOUNT_SHIFT = 8,
    D3D_FVF_TEXCOUNT_MASK = 0xf00,
};

/* ---- vertices ---- */

/* the layout of one Direct3D 7 flexible vertex format (offsets in bytes, -1 when absent) */
struct Sdl3Fvf {
    u32 stride;
    int transformed; /* XYZRHW */
    int diffuse, specular, tex;
};

static void Sdl3_ParseFvf(u32 fvf, Sdl3Fvf *out)
{
    u32 offset;
    u32 position = fvf & D3D_FVF_POSITION_MASK;
    out->transformed = position == RD_FVF_XYZRHW;
    offset = out->transformed ? 16 : 12; /* XYZ, and the XYZB1..5 blend weights the game never uses */
    if (position > RD_FVF_XYZRHW)
        offset += 4 * ((position - RD_FVF_XYZRHW) / 2);
    if (fvf & D3D_FVF_NORMAL)
        offset += 12;
    if (fvf & D3D_FVF_PSIZE)
        offset += 4;
    out->diffuse = -1;
    out->specular = -1;
    out->tex = -1;
    if (fvf & RD_FVF_DIFFUSE) {
        out->diffuse = (int)offset;
        offset += 4;
    }
    if (fvf & RD_FVF_SPECULAR) {
        out->specular = (int)offset;
        offset += 4;
    }
    if ((fvf & D3D_FVF_TEXCOUNT_MASK) >> D3D_FVF_TEXCOUNT_SHIFT) {
        out->tex = (int)offset;
        offset += 8 * ((fvf & D3D_FVF_TEXCOUNT_MASK) >> D3D_FVF_TEXCOUNT_SHIFT);
    }
    out->stride = offset;
}

/* what the GPU gets: a pre-transformed vertex (game.hlsl's VSIn) */
struct Sdl3Vertex {
    float x, y, z, rhw;
    u32 diffuse, specular; /* D3DCOLOR */
    float u, v;
};

/* the viewport transform after world x view x projection: Direct3D 7's, for a viewport of width x height, z 0..1 */
static void Sdl3_Transform(const float *m, const float *in, float width, float height, float *out)
{
    float x = in[0] * m[0] + in[1] * m[4] + in[2] * m[8] + m[12];
    float y = in[0] * m[1] + in[1] * m[5] + in[2] * m[9] + m[13];
    float z = in[0] * m[2] + in[1] * m[6] + in[2] * m[10] + m[14];
    float w = in[0] * m[3] + in[1] * m[7] + in[2] * m[11] + m[15];
    float rhw = 1.0f / w;
    out[0] = (1.0f + x * rhw) * width * 0.5f;
    out[1] = (1.0f - y * rhw) * height * 0.5f;
    out[2] = z * rhw;
    out[3] = rhw;
}

static void Sdl3_MatMul(const float *a, const float *b, float *out)
{
    int r, c;
    for (r = 0; r < 4; r++)
        for (c = 0; c < 4; c++)
            out[r * 4 + c] = a[r * 4] * b[c] + a[r * 4 + 1] * b[4 + c] + a[r * 4 + 2] * b[8 + c] + a[r * 4 + 3] * b[12 + c];
}

class Sdl3RenderDevice;

/* a vertex buffer: plain memory (the game only transforms into it and reads it back) */
class Sdl3VertexBuffer : public RdVertexBuffer {
public:
    Sdl3VertexBuffer(u32 fvf, u32 count) : fvf(fvf), count(count)
    {
        Sdl3_ParseFvf(fvf, &layout);
        data = (u8 *)SDL_calloc(count ? count : 1, layout.stride);
    }
    long Lock(u32 flags, void **out, u32 *size)
    {
        (void)flags;
        *out = data;
        if (size)
            *size = count * layout.stride;
        return 0;
    }
    long Unlock() { return 0; }
    long ProcessVertices(u32 op, u32 dstIndex, u32 n, RdVertexBuffer *src, u32 srcIndex, RenderDevice *device,
                         u32 flags);
    u32 Release()
    {
        delete this;
        return 0;
    }

    u32 fvf, count;
    Sdl3Fvf layout;
    u8 *data;

private:
    ~Sdl3VertexBuffer() { SDL_free(data); }
};

/* ---- textures ---- */
struct Sdl3Texture {
    SDL_GPUTexture *gpu;
    u16 *pixels; /* the texels, rows packed: 16-bit, or 32-bit RGBA for TEXFMT_RGBA8 (Sdl3_TexelBytes); 0 for a frame
                  * capture (on the GPU only) */
    u32 width, height, format;
    int dirty;         /* the texels changed since the last upload */
    int lockedToWrite; /* locked with write access: dirty at Unlock */
    u32 drawnInBatch;  /* the batch (Flush count) that last drew with it */
};

/* texels to RGBA8, as Direct3D reads the three formats (565 has alpha 1) */
/* bytes per texel of a texture format */
static u32 Sdl3_TexelBytes(u32 format)
{
    return format == TEXFMT_RGBA8 ? 4 : 2;
}

static void Sdl3_ConvertTexels(const Sdl3Texture *t, u8 *out)
{
    u32 i, n = t->width * t->height;
    const u16 *p = t->pixels;
    if (t->format == TEXFMT_RGBA8) {
        SDL_memcpy(out, t->pixels, (size_t)n * 4); /* already what the GPU texture holds */
        return;
    }
    for (i = 0; i < n; i++, out += 4) {
        u32 c = p[i];
        switch (t->format) {
            case TEXFMT_ARGB1555:
                out[0] = (u8)(((c >> 10) & 31) * 255 / 31);
                out[1] = (u8)(((c >> 5) & 31) * 255 / 31);
                out[2] = (u8)((c & 31) * 255 / 31);
                out[3] = (c & 0x8000) ? 255 : 0;
                break;
            case TEXFMT_ARGB4444:
                out[0] = (u8)(((c >> 8) & 15) * 17);
                out[1] = (u8)(((c >> 4) & 15) * 17);
                out[2] = (u8)((c & 15) * 17);
                out[3] = (u8)(((c >> 12) & 15) * 17);
                break;
            default: /* TEXFMT_RGB565 */
                out[0] = (u8)(((c >> 11) & 31) * 255 / 31);
                out[1] = (u8)(((c >> 5) & 63) * 255 / 63);
                out[2] = (u8)((c & 31) * 255 / 31);
                out[3] = 255;
        }
    }
}

/* ---- recorded draws ---- */

/* pipeline key bits */
enum {
    PK_LINES = 1 << 0,
    PK_BLEND = 1 << 1,
    PK_SRC_SHIFT = 2, /* 4 bits: the D3DBLEND */
    PK_DST_SHIFT = 6, /* 4 bits */
    PK_ZTEST = 1 << 10,
    PK_ZWRITE = 1 << 11,
    PK_COUNT = 1 << 12,
};

struct Sdl3PsParams {
    float fogColor[4];
    float mode[4]; /* texture mode (0: diffuse, 1: MODULATE, 2: MODULATE2X), alpha test, fog, specular */
};

struct Sdl3Draw {
    u32 pipelineKey;
    Sdl3Texture *texture;
    int linear;
    Sdl3PsParams ps;
    u32 firstVertex, vertexCount;
};

/* the Direct3D 7 state the game changes */
struct Sdl3State {
    int blend;
    u32 srcBlend, dstBlend;
    int alphaTest, zEnable, zWrite, specular, fog, linear;
    u32 cull;
    int textured; /* 0: diffuse (SELECTARG1 DIFFUSE or DISABLE), 1: MODULATE, 2: MODULATE2X */
    u32 fogColor;
    Sdl3Texture *texture;
};

static SDL_GPUBlendFactor Sdl3_BlendFactor(u32 d3dBlend)
{
    switch (d3dBlend) {
        case D3D_BLEND_ZERO:
            return SDL_GPU_BLENDFACTOR_ZERO;
        case D3D_BLEND_SRCCOLOR:
            return SDL_GPU_BLENDFACTOR_SRC_COLOR;
        case D3D_BLEND_INVSRCCOLOR:
            return SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_COLOR;
        case D3D_BLEND_SRCALPHA:
            return SDL_GPU_BLENDFACTOR_SRC_ALPHA;
        case D3D_BLEND_INVSRCALPHA:
            return SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
        case D3D_BLEND_DESTALPHA:
            return SDL_GPU_BLENDFACTOR_DST_ALPHA;
        case D3D_BLEND_INVDESTALPHA:
            return SDL_GPU_BLENDFACTOR_ONE_MINUS_DST_ALPHA;
        case D3D_BLEND_DESTCOLOR:
            return SDL_GPU_BLENDFACTOR_DST_COLOR;
        case D3D_BLEND_INVDESTCOLOR:
            return SDL_GPU_BLENDFACTOR_ONE_MINUS_DST_COLOR;
        case D3D_BLEND_SRCALPHASAT:
            return SDL_GPU_BLENDFACTOR_SRC_ALPHA_SATURATE;
        default:
            return SDL_GPU_BLENDFACTOR_ONE;
    }
}

/* ---- the device ---- */
class Sdl3RenderDevice : public RenderDevice {
public:
    Sdl3RenderDevice(SDL_GPUDevice *gpu, SDL_Window *window, u32 width, u32 height);
    ~Sdl3RenderDevice();
    int Init(); /* 0 when a GPU object could not be made */
    const char *Name() const { return name; }

    long BeginScene() { return 0; }
    long EndScene() { return 0; }
    long Clear(u32 rectCount, void *rects, u32 flags, u32 color, float z, u32 stencil);
    long Present();

    void SetStateFlags(u32 rsf);
    void ClearStateFlags(u32 rsf);
    void SetTextureModulation2X(bool enabled) { state.textured = enabled ? 2 : 1; }
    void EnableAlphaBlend(int on) { state.blend = on != 0; }
    void SetBlendFunc(u32 src, u32 dst)
    {
        state.srcBlend = src;
        state.dstBlend = dst;
    }
    void SetFog(u32 color, float start, float end)
    {
        (void)start; /* vertex fog: the game computes the factor into the specular alpha */
        (void)end;
        state.fogColor = color;
    }
    void SetFogColor(u32 color) { state.fogColor = color; }

    long SetTransform(u32 which, void *matrix);
    long SetTexture(u32 stage, RdTexture *texture)
    {
        if (stage == 0)
            state.texture = (Sdl3Texture *)texture;
        return 0;
    }
    long DrawPrimitive(u32 primitive, u32 fvf, void *vertices, u32 count, u32 flags);

    long CreateVertexBuffer(u32 caps, u32 fvf, u32 vertexCount, RdVertexBuffer **out)
    {
        (void)caps;
        *out = new Sdl3VertexBuffer(fvf, vertexCount);
        return 0;
    }

    s32 CreateTexture(u32 width, u32 height, u32 format, RdTexture **out, u32 *outWidth, u32 *outHeight);
    long LockTexture(RdTexture *texture, u32 access, RdLockedRect *out);
    long UnlockTexture(RdTexture *texture);
    long CopyTexture(RdTexture *dst, u32 x, u32 y, RdTexture *src, RdRect *srcRect);
    void ReleaseTexture(RdTexture *texture);
    int IsTextureLost(RdTexture *texture)
    {
        (void)texture;
        return 0;
    }
    long RestoreTexture(RdTexture *texture)
    {
        (void)texture;
        return 0;
    }
    long CreateFrameCaptureTexture(u32 width, u32 height, RdTexture **out, u32 *outWidth, u32 *outHeight);
    long CopyFrameToTexture(RdTexture *dst, RdRect *dstRect, int fromShown);

    /* world x view x projection, for ProcessVertices */
    const float *Combined();
    float Width() const { return (float)width; }
    float Height() const { return (float)height; }

private:
    SDL_GPUCommandBuffer *Commands();
    void Flush();
    void BeginBatch() { batch++; }
    SDL_GPUGraphicsPipeline *Pipeline(u32 key);
    void MarkUsed(Sdl3Texture *t) { t->drawnInBatch = batch; }
    int DrawnInBatch(const Sdl3Texture *t) const { return t->drawnInBatch == batch && drawCount; }
    void WaitUntilUnused(Sdl3Texture *t)
    {
        if (DrawnInBatch(t))
            Flush();
    }
    Sdl3Vertex *AllocVertices(u32 n);
    int TrackTexture(Sdl3Texture *t);
    void UntrackTexture(Sdl3Texture *t);
    void DrawPerfOverlay();

    SDL_GPUDevice *gpu;
    SDL_Window *window;
    u32 width, height;
    char name[64];

    SDL_GPUShader *vertexShader, *pixelShader;
    SDL_GPUGraphicsPipeline *pipelines[PK_COUNT];
    SDL_GPUSampler *pointSampler, *linearSampler;
    SDL_GPUTextureFormat depthFormat;
    SDL_GPUTexture *frames[2];
    int current;
    SDL_GPUTexture *depth;
    Sdl3Texture white; /* bound when the game has no texture set */
    Sdl3Texture font;  /* the performance overlay's glyphs, made the first time it is shown */

    SDL_GPUCommandBuffer *commands;
    SDL_GPUBuffer *vertexBuffer;
    u32 vertexBufferSize;
    SDL_GPUTransferBuffer *transfer;
    u32 transferSize;

    /* the recorded batch */
    u32 batch;
    Sdl3Vertex *vertices;
    u32 vertexCount, vertexCapacity;
    Sdl3Draw *draws;
    u32 drawCount, drawCapacity;
    u32 frameDraws, frameVertices; /* what the frame submitted so far, for the performance overlay */
    int clearColor, clearDepth;
    SDL_FColor clearColorValue;
    float clearDepthValue;

    /* textures, for uploading the changed ones */
    Sdl3Texture **textures;
    u32 textureCount, textureCapacity;

    Sdl3State state;
    float matrices[3][16]; /* world, view, projection */
    float combined[16];
    int combinedValid;
};

Sdl3RenderDevice::Sdl3RenderDevice(SDL_GPUDevice *gpu, SDL_Window *window, u32 width, u32 height)
    : gpu(gpu), window(window), width(width), height(height)
{
    int i;
    SDL_snprintf(name, sizeof(name), "SDL3 GPU (%s)", SDL_GetGPUDeviceDriver(gpu));
    vertexShader = pixelShader = 0;
    for (i = 0; i < PK_COUNT; i++)
        pipelines[i] = 0;
    pointSampler = linearSampler = 0;
    frames[0] = frames[1] = 0;
    current = 0;
    depth = 0;
    SDL_zero(white);
    SDL_zero(font);
    commands = 0;
    vertexBuffer = 0;
    vertexBufferSize = 0;
    transfer = 0;
    transferSize = 0;
    batch = 1;
    vertices = 0;
    vertexCount = vertexCapacity = 0;
    draws = 0;
    drawCount = drawCapacity = 0;
    frameDraws = frameVertices = 0;
    clearColor = clearDepth = 0;
    clearDepthValue = 1.0f;
    SDL_zero(clearColorValue);
    textures = 0;
    textureCount = textureCapacity = 0;

    /* Direct3D 7's defaults (the game only changes what its RSF_* bundles touch) */
    SDL_zero(state);
    state.srcBlend = D3D_BLEND_ONE;
    state.dstBlend = D3D_BLEND_ZERO;
    state.zEnable = 1; /* the render target has a z buffer */
    state.zWrite = 1;
    state.cull = D3D_CULL_CCW;
    state.textured = 1;
    for (i = 0; i < 3; i++) {
        SDL_zeroa(matrices[i]);
        matrices[i][0] = matrices[i][5] = matrices[i][10] = matrices[i][15] = 1.0f;
    }
    combinedValid = 0;
}

int Sdl3RenderDevice::Init()
{
    SDL_GPUShaderFormat formats = SDL_GetGPUShaderFormats(gpu);
    SDL_GPUShaderCreateInfo vs, ps;
    SDL_GPUSamplerCreateInfo sampler;
    SDL_GPUTextureCreateInfo tex;
    int i;

    SDL_zero(vs);
    SDL_zero(ps);
    vs.stage = SDL_GPU_SHADERSTAGE_VERTEX;
    vs.num_uniform_buffers = 1;
    ps.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
    ps.num_samplers = 1;
    ps.num_uniform_buffers = 1;
    vs.entrypoint = "VSMain";
    ps.entrypoint = "PSMain";
    if (formats & SDL_GPU_SHADERFORMAT_SPIRV) {
        vs.format = ps.format = SDL_GPU_SHADERFORMAT_SPIRV;
        vs.code = g_sdl3_vs_spirv;
        vs.code_size = sizeof(g_sdl3_vs_spirv);
        ps.code = g_sdl3_ps_spirv;
        ps.code_size = sizeof(g_sdl3_ps_spirv);
    } else if (formats & SDL_GPU_SHADERFORMAT_DXBC) {
        vs.format = ps.format = SDL_GPU_SHADERFORMAT_DXBC;
        vs.code = g_sdl3_vs_dxbc;
        vs.code_size = sizeof(g_sdl3_vs_dxbc);
        ps.code = g_sdl3_ps_dxbc;
        ps.code_size = sizeof(g_sdl3_ps_dxbc);
    } else if (formats & SDL_GPU_SHADERFORMAT_MSL) {
        vs.format = ps.format = SDL_GPU_SHADERFORMAT_MSL;
        vs.code = (const Uint8 *)g_sdl3_msl;
        vs.code_size = SDL_strlen(g_sdl3_msl);
        ps.code = vs.code;
        ps.code_size = vs.code_size;
    } else {
        return SDL_SetError("no shader format this renderer has");
    }
    vertexShader = SDL_CreateGPUShader(gpu, &vs);
    pixelShader = SDL_CreateGPUShader(gpu, &ps);
    if (!vertexShader || !pixelShader)
        return 0;

    SDL_zero(sampler);
    sampler.min_filter = sampler.mag_filter = SDL_GPU_FILTER_NEAREST;
    sampler.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
    sampler.address_mode_u = sampler.address_mode_v = sampler.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
    pointSampler = SDL_CreateGPUSampler(gpu, &sampler);
    sampler.min_filter = sampler.mag_filter = SDL_GPU_FILTER_LINEAR;
    linearSampler = SDL_CreateGPUSampler(gpu, &sampler);
    if (!pointSampler || !linearSampler)
        return 0;

    /* the frames: RGBA8 (the original's were 16-bit), a sampler source for the scaled blit into the window */
    SDL_zero(tex);
    tex.type = SDL_GPU_TEXTURETYPE_2D;
    tex.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    tex.usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER;
    tex.width = width;
    tex.height = height;
    tex.layer_count_or_depth = 1;
    tex.num_levels = 1;
    for (i = 0; i < 2; i++)
        if (!(frames[i] = SDL_CreateGPUTexture(gpu, &tex)))
            return 0;
    /* 16-bit, as the original's (the first z buffer format the driver lists), so that surfaces drawn over each other
     * at nearly the same depth resolve as they did */
    depthFormat = SDL_GPU_TEXTUREFORMAT_D16_UNORM;
    tex.format = depthFormat;
    tex.usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET;
    if (!(depth = SDL_CreateGPUTexture(gpu, &tex)))
        return 0;

    /* a white texel (4444: 0xffff), bound when there is no texture: MODULATE then gives the diffuse colour */
    white.width = white.height = 1;
    white.format = TEXFMT_ARGB4444;
    white.pixels = (u16 *)SDL_malloc(2);
    white.pixels[0] = 0xffff;
    tex.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    tex.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
    tex.width = tex.height = 1;
    if (!(white.gpu = SDL_CreateGPUTexture(gpu, &tex)))
        return 0;
    white.dirty = 1;
    TrackTexture(&white);

    /* both frames black, as the original's first Clear */
    clearColor = clearDepth = 1;
    clearColorValue.a = 1.0f;
    Flush();
    current ^= 1;
    clearColor = clearDepth = 1;
    Flush();
    current ^= 1;
    return 1;
}

Sdl3RenderDevice::~Sdl3RenderDevice()
{
    int i;
    if (commands)
        SDL_CancelGPUCommandBuffer(commands);
    SDL_WaitForGPUIdle(gpu);
    for (i = 0; i < PK_COUNT; i++)
        if (pipelines[i])
            SDL_ReleaseGPUGraphicsPipeline(gpu, pipelines[i]);
    if (vertexShader)
        SDL_ReleaseGPUShader(gpu, vertexShader);
    if (pixelShader)
        SDL_ReleaseGPUShader(gpu, pixelShader);
    if (pointSampler)
        SDL_ReleaseGPUSampler(gpu, pointSampler);
    if (linearSampler)
        SDL_ReleaseGPUSampler(gpu, linearSampler);
    for (i = 0; i < 2; i++)
        if (frames[i])
            SDL_ReleaseGPUTexture(gpu, frames[i]);
    if (depth)
        SDL_ReleaseGPUTexture(gpu, depth);
    if (white.gpu)
        SDL_ReleaseGPUTexture(gpu, white.gpu);
    SDL_free(white.pixels);
    if (font.gpu)
        SDL_ReleaseGPUTexture(gpu, font.gpu);
    SDL_free(font.pixels);
    if (vertexBuffer)
        SDL_ReleaseGPUBuffer(gpu, vertexBuffer);
    if (transfer)
        SDL_ReleaseGPUTransferBuffer(gpu, transfer);
    SDL_free(vertices);
    SDL_free(draws);
    /* the game's textures still alive (the menu noise is deleted after the device) are left to the device */
    SDL_free(textures);
    SDL_ReleaseWindowFromGPUDevice(gpu, window); /* the window itself is the platform layer's */
    SDL_DestroyGPUDevice(gpu);
}

SDL_GPUCommandBuffer *Sdl3RenderDevice::Commands()
{
    if (!commands)
        commands = SDL_AcquireGPUCommandBuffer(gpu);
    return commands;
}

int Sdl3RenderDevice::TrackTexture(Sdl3Texture *t)
{
    if (textureCount == textureCapacity) {
        u32 capacity = textureCapacity ? textureCapacity * 2 : 256;
        Sdl3Texture **grown = (Sdl3Texture **)SDL_realloc(textures, capacity * sizeof(*textures));
        if (!grown)
            return 0;
        textures = grown;
        textureCapacity = capacity;
    }
    textures[textureCount++] = t;
    return 1;
}

void Sdl3RenderDevice::UntrackTexture(Sdl3Texture *t)
{
    u32 i;
    for (i = 0; i < textureCount; i++)
        if (textures[i] == t) {
            textures[i] = textures[--textureCount];
            return;
        }
}

/* ---- state: Render_SetStateFlags / ClearStateFlags, as the original sets Direct3D's (src/app/d3dapp_inlines.h) ---- */
void Sdl3RenderDevice::SetStateFlags(u32 flags)
{
    if (flags & RSF_BLEND_ALPHA) {
        state.blend = 1;
        state.srcBlend = D3D_BLEND_INVSRCALPHA;
        state.dstBlend = D3D_BLEND_SRCALPHA;
    }
    if (flags & RSF_BLEND_ADD) {
        state.blend = 1;
        state.srcBlend = D3D_BLEND_INVSRCALPHA;
        state.dstBlend = D3D_BLEND_ONE;
    }
    if (flags & RSF_ALPHATEST)
        state.alphaTest = 1;
    if (flags & RSF_SPECULAR)
        state.specular = 1;
    if (flags & RSF_CULL_CW)
        state.cull = D3D_CULL_CW;
    if (flags & RSF_CULL_CCW)
        state.cull = D3D_CULL_CCW;
    if (flags & RSF_ZTEST)
        state.zEnable = 1;
    /* the original turns z testing on only when setting the z write state fails, which it does not */
    if (flags & RSF_ZWRITE_ON)
        state.zWrite = 1;
    if (flags & RSF_ZWRITE_OFF)
        state.zWrite = 0;
    state.textured = (flags & RSF_TEXTURED) ? 2 : 0; /* ColorFix: MODULATE2X; else SELECTARG1 DIFFUSE */
    if (flags & RSF_FILTER_LINEAR)
        state.linear = 1;
    if (flags & RSF_FOG)
        state.fog = 1;
    /* RSF_ANTIALIAS, CLIPPLANE (no effect on pre-transformed vertices), DITHER, LIGHTING, COLORVERTEX: nothing to do */
}

void Sdl3RenderDevice::ClearStateFlags(u32 flags)
{
    if ((flags & RSF_BLEND_ALPHA) || (flags & RSF_BLEND_ADD))
        state.blend = 0;
    if (flags & RSF_ALPHATEST)
        state.alphaTest = 0;
    if (flags & RSF_SPECULAR)
        state.specular = 0;
    if ((flags & RSF_CULL_CW) || (flags & RSF_CULL_CCW))
        state.cull = D3D_CULL_NONE;
    if (flags & RSF_ZTEST)
        state.zEnable = 0;
    if (flags & RSF_ZWRITE_ON)
        state.zWrite = 0;
    if (flags & RSF_ZWRITE_OFF)
        state.zWrite = 1;
    if (flags & RSF_TEXTURED)
        state.textured = 0; /* COLOROP DISABLE: the diffuse colour */
    if (flags & RSF_FILTER_LINEAR)
        state.linear = 0;
    if (flags & RSF_FOG)
        state.fog = 0;
}

long Sdl3RenderDevice::SetTransform(u32 which, void *matrix)
{
    if (which < RD_TRANSFORM_WORLD || which > RD_TRANSFORM_PROJECTION)
        return SDL3_E_FAIL;
    SDL_memcpy(matrices[which - RD_TRANSFORM_WORLD], matrix, sizeof(matrices[0]));
    combinedValid = 0;
    return 0;
}

const float *Sdl3RenderDevice::Combined()
{
    if (!combinedValid) {
        float worldView[16];
        Sdl3_MatMul(matrices[0], matrices[1], worldView);
        Sdl3_MatMul(worldView, matrices[2], combined);
        combinedValid = 1;
    }
    return combined;
}

long Sdl3VertexBuffer::ProcessVertices(u32 op, u32 dstIndex, u32 n, RdVertexBuffer *srcBuffer, u32 srcIndex,
                                       RenderDevice *device, u32 flags)
{
    Sdl3VertexBuffer *src = (Sdl3VertexBuffer *)srcBuffer;
    Sdl3RenderDevice *dev = (Sdl3RenderDevice *)device;
    const float *m;
    u32 i;
    (void)op; /* RD_VOP_TRANSFORM: the game never asks for lighting or clipping */
    if (!layout.transformed || src->layout.transformed || dstIndex + n > count || srcIndex + n > src->count)
        return SDL3_E_FAIL;
    m = dev->Combined();
    for (i = 0; i < n; i++) {
        const u8 *in = src->data + (srcIndex + i) * src->layout.stride;
        u8 *out = data + (dstIndex + i) * layout.stride;
        Sdl3_Transform(m, (const float *)in, dev->Width(), dev->Height(), (float *)out);
        if (!(flags & RD_PV_DONOTCOPYDATA)) {
            if (layout.diffuse >= 0 && src->layout.diffuse >= 0)
                SDL_memcpy(out + layout.diffuse, in + src->layout.diffuse, 4);
            if (layout.specular >= 0 && src->layout.specular >= 0)
                SDL_memcpy(out + layout.specular, in + src->layout.specular, 4);
            if (layout.tex >= 0 && src->layout.tex >= 0)
                SDL_memcpy(out + layout.tex, in + src->layout.tex, 8);
        }
    }
    return 0;
}

/* ---- draws ---- */
Sdl3Vertex *Sdl3RenderDevice::AllocVertices(u32 n)
{
    if (vertexCount + n > vertexCapacity) {
        u32 capacity = vertexCapacity ? vertexCapacity : 16384;
        Sdl3Vertex *grown;
        while (capacity < vertexCount + n)
            capacity *= 2;
        grown = (Sdl3Vertex *)SDL_realloc(vertices, capacity * sizeof(Sdl3Vertex));
        if (!grown)
            return 0;
        vertices = grown;
        vertexCapacity = capacity;
    }
    return vertices + vertexCount;
}

long Sdl3RenderDevice::DrawPrimitive(u32 primitive, u32 fvf, void *data, u32 count, u32 flags)
{
    Sdl3Fvf layout;
    Sdl3Vertex *out;
    const u8 *in = (const u8 *)data;
    u32 i, kept = 0, key;
    int lines = primitive == RD_PRIM_LINELIST;
    Sdl3Draw *last;
    Sdl3Texture *texture;
    Sdl3PsParams ps;
    (void)flags;

    if (!lines && primitive != RD_PRIM_TRIANGLELIST)
        return SDL3_E_FAIL; /* the game draws nothing else */
    count -= count % (lines ? 2 : 3);
    if (!count)
        return 0;
    Sdl3_ParseFvf(fvf, &layout);
    if (!(out = AllocVertices(count)))
        return SDL3_E_FAIL;
    for (i = 0; i < count; i++, in += layout.stride) {
        Sdl3Vertex *v = out + kept + i % (lines ? 2 : 3);
        if (layout.transformed)
            SDL_memcpy(&v->x, in, 16);
        else
            Sdl3_Transform(Combined(), (const float *)in, (float)width, (float)height, &v->x);
        v->diffuse = layout.diffuse >= 0 ? *(const u32 *)(in + layout.diffuse) : 0xffffffff;
        v->specular = layout.specular >= 0 ? *(const u32 *)(in + layout.specular) : 0xff000000;
        if (layout.tex >= 0) {
            v->u = ((const float *)(in + layout.tex))[0];
            v->v = ((const float *)(in + layout.tex))[1];
        } else {
            v->u = v->v = 0.0f;
        }
        if (lines) {
            if (i % 2 == 1)
                kept += 2;
        } else if (i % 3 == 2) {
            /* Direct3D's culling: the winding on the screen (y down), CW = positive area; a triangle reaching behind
             * the eye (rhw <= 0) has no meaningful screen winding and is left to the clipper */
            Sdl3Vertex *t = out + kept;
            float area = (t[1].x - t[0].x) * (t[2].y - t[0].y) - (t[2].x - t[0].x) * (t[1].y - t[0].y);
            int behind = t[0].rhw <= 0.0f || t[1].rhw <= 0.0f || t[2].rhw <= 0.0f;
            if (behind ||
                (!(state.cull == D3D_CULL_CW && area > 0.0f) && !(state.cull == D3D_CULL_CCW && area < 0.0f)))
                kept += 3;
        }
    }
    if (!kept)
        return 0;

    key = (lines ? PK_LINES : 0) | (state.zEnable ? PK_ZTEST : 0) | (state.zEnable && state.zWrite ? PK_ZWRITE : 0);
    if (state.blend)
        key |= PK_BLEND | ((state.srcBlend & 15) << PK_SRC_SHIFT) | ((state.dstBlend & 15) << PK_DST_SHIFT);
    texture = state.texture ? state.texture : &white;
    SDL_zero(ps);
    ps.fogColor[0] = ((state.fogColor >> 16) & 0xff) / 255.0f;
    ps.fogColor[1] = ((state.fogColor >> 8) & 0xff) / 255.0f;
    ps.fogColor[2] = (state.fogColor & 0xff) / 255.0f;
    ps.fogColor[3] = 1.0f;
    ps.mode[0] = (float)state.textured;
    ps.mode[1] = state.alphaTest ? 1.0f : 0.0f;
    ps.mode[2] = state.fog ? 1.0f : 0.0f;
    ps.mode[3] = state.specular ? 1.0f : 0.0f;
    if (!state.textured)
        texture = &white; /* not sampled, but bound */
    MarkUsed(texture);

    /* the same state as the draw before: one draw */
    last = drawCount ? &draws[drawCount - 1] : 0;
    if (last && last->pipelineKey == key && last->texture == texture && last->linear == state.linear &&
        !SDL_memcmp(&last->ps, &ps, sizeof(ps)) && last->firstVertex + last->vertexCount == vertexCount) {
        last->vertexCount += kept;
    } else {
        if (drawCount == drawCapacity) {
            u32 capacity = drawCapacity ? drawCapacity * 2 : 1024;
            Sdl3Draw *grown = (Sdl3Draw *)SDL_realloc(draws, capacity * sizeof(Sdl3Draw));
            if (!grown)
                return SDL3_E_FAIL;
            draws = grown;
            drawCapacity = capacity;
        }
        last = &draws[drawCount++];
        last->pipelineKey = key;
        last->texture = texture;
        last->linear = state.linear;
        last->ps = ps;
        last->firstVertex = vertexCount;
        last->vertexCount = kept;
    }
    vertexCount += kept;
    return 0;
}

long Sdl3RenderDevice::Clear(u32 rectCount, void *rects, u32 flags, u32 color, float z, u32 stencil)
{
    (void)rectCount; /* the game always clears the whole target */
    (void)rects;
    (void)stencil;
    if (drawCount)
        Flush(); /* what was drawn before goes under the clear */
    if (flags & RD_CLEAR_TARGET) {
        clearColor = 1;
        clearColorValue.r = ((color >> 16) & 0xff) / 255.0f;
        clearColorValue.g = ((color >> 8) & 0xff) / 255.0f;
        clearColorValue.b = (color & 0xff) / 255.0f;
        clearColorValue.a = 1.0f;
        {
            const char *presentTest = SDL_getenv("SDW_PRESENT_TEST");
            if (presentTest && presentTest[0] == '3' && presentTest[1] == 0) {
                clearColorValue.r = 0.0f;
                clearColorValue.g = 1.0f;
                clearColorValue.b = 0.0f;
            }
        }
    }
    if (flags & RD_CLEAR_ZBUFFER) {
        clearDepth = 1;
        clearDepthValue = z;
    }
    return 0;
}

SDL_GPUGraphicsPipeline *Sdl3RenderDevice::Pipeline(u32 key)
{
    SDL_GPUGraphicsPipelineCreateInfo info;
    SDL_GPUVertexBufferDescription buffer;
    SDL_GPUVertexAttribute attributes[4];
    SDL_GPUColorTargetDescription target;

    if (pipelines[key])
        return pipelines[key];
    SDL_zero(info);
    SDL_zero(buffer);
    SDL_zeroa(attributes);
    SDL_zero(target);
    buffer.slot = 0;
    buffer.pitch = sizeof(Sdl3Vertex);
    buffer.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
    attributes[0].location = 0;
    attributes[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
    attributes[0].offset = 0;
    attributes[1].location = 1;
    attributes[1].format = SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM;
    attributes[1].offset = 16;
    attributes[2].location = 2;
    attributes[2].format = SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM;
    attributes[2].offset = 20;
    attributes[3].location = 3;
    attributes[3].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
    attributes[3].offset = 24;
    info.vertex_shader = vertexShader;
    info.fragment_shader = pixelShader;
    info.vertex_input_state.vertex_buffer_descriptions = &buffer;
    info.vertex_input_state.num_vertex_buffers = 1;
    info.vertex_input_state.vertex_attributes = attributes;
    info.vertex_input_state.num_vertex_attributes = 4;
    info.primitive_type = (key & PK_LINES) ? SDL_GPU_PRIMITIVETYPE_LINELIST : SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
    info.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
    info.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE; /* culled on the CPU (DrawPrimitive) */
    info.rasterizer_state.enable_depth_clip = true; /* the near plane cuts triangles reaching behind the eye */
    info.depth_stencil_state.compare_op = SDL_GPU_COMPAREOP_LESS_OR_EQUAL; /* D3DCMP_LESSEQUAL, the default */
    info.depth_stencil_state.enable_depth_test = (key & PK_ZTEST) != 0;
    info.depth_stencil_state.enable_depth_write = (key & PK_ZWRITE) != 0;
    target.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    /* the frame's alpha stays 1, as the original's 16-bit back buffer had none (DESTALPHA is then 1) */
    target.blend_state.enable_color_write_mask = true;
    target.blend_state.color_write_mask = SDL_GPU_COLORCOMPONENT_R | SDL_GPU_COLORCOMPONENT_G | SDL_GPU_COLORCOMPONENT_B;
    if (key & PK_BLEND) {
        SDL_GPUBlendFactor src = Sdl3_BlendFactor((key >> PK_SRC_SHIFT) & 15);
        SDL_GPUBlendFactor dst = Sdl3_BlendFactor((key >> PK_DST_SHIFT) & 15);
        target.blend_state.enable_blend = true;
        target.blend_state.src_color_blendfactor = target.blend_state.src_alpha_blendfactor = src;
        target.blend_state.dst_color_blendfactor = target.blend_state.dst_alpha_blendfactor = dst;
        target.blend_state.color_blend_op = target.blend_state.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
    }
    info.target_info.color_target_descriptions = &target;
    info.target_info.num_color_targets = 1;
    info.target_info.depth_stencil_format = depthFormat;
    info.target_info.has_depth_stencil_target = true;
    pipelines[key] = SDL_CreateGPUGraphicsPipeline(gpu, &info);
    if (!pipelines[key])
        SDL_Log("SheepD3D: pipeline 0x%x: %s", (unsigned)key, SDL_GetError());
    return pipelines[key];
}

/* Records the batch into the command buffer: the changed textures and the vertices are uploaded, then one render pass
 * draws the batch after its pending clears. */
void Sdl3RenderDevice::Flush()
{
    SDL_GPUCommandBuffer *cmd;
    u32 i, uploadBytes = vertexCount * sizeof(Sdl3Vertex), vertexBytes = uploadBytes;
    int uploads = 0;

    for (i = 0; i < textureCount; i++)
        if (textures[i]->dirty) {
            uploadBytes += textures[i]->width * textures[i]->height * 4;
            uploads = 1;
        }
    if (!drawCount && !clearColor && !clearDepth && !uploads)
        return;
    if (!(cmd = Commands()))
        return;

    if (uploadBytes) {
        SDL_GPUCopyPass *copy;
        u8 *mapped;
        u32 offset = 0;
        if (uploadBytes > transferSize) {
            SDL_GPUTransferBufferCreateInfo info;
            if (transfer)
                SDL_ReleaseGPUTransferBuffer(gpu, transfer);
            transferSize = uploadBytes + uploadBytes / 2;
            SDL_zero(info);
            info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
            info.size = transferSize;
            transfer = SDL_CreateGPUTransferBuffer(gpu, &info);
        }
        if (vertexBytes > vertexBufferSize) {
            SDL_GPUBufferCreateInfo info;
            if (vertexBuffer)
                SDL_ReleaseGPUBuffer(gpu, vertexBuffer);
            vertexBufferSize = vertexBytes + vertexBytes / 2;
            SDL_zero(info);
            info.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
            info.size = vertexBufferSize;
            vertexBuffer = SDL_CreateGPUBuffer(gpu, &info);
        }
        if (!transfer || (vertexBytes && !vertexBuffer) ||
            !(mapped = (u8 *)SDL_MapGPUTransferBuffer(gpu, transfer, true))) {
            SDL_Log("SheepD3D: upload: %s", SDL_GetError());
            vertexCount = drawCount = 0;
            return;
        }
        if (vertexBytes)
            SDL_memcpy(mapped, vertices, vertexBytes);
        offset = vertexBytes;
        for (i = 0; i < textureCount; i++)
            if (textures[i]->dirty) {
                Sdl3_ConvertTexels(textures[i], mapped + offset);
                offset += textures[i]->width * textures[i]->height * 4;
            }
        SDL_UnmapGPUTransferBuffer(gpu, transfer);

        copy = SDL_BeginGPUCopyPass(cmd);
        if (vertexBytes) {
            SDL_GPUTransferBufferLocation from;
            SDL_GPUBufferRegion to;
            from.transfer_buffer = transfer;
            from.offset = 0;
            to.buffer = vertexBuffer;
            to.offset = 0;
            to.size = vertexBytes;
            SDL_UploadToGPUBuffer(copy, &from, &to, true); /* cycle: an earlier pass may still read the old vertices */
        }
        offset = vertexBytes;
        for (i = 0; i < textureCount; i++) {
            Sdl3Texture *t = textures[i];
            SDL_GPUTextureTransferInfo from;
            SDL_GPUTextureRegion to;
            if (!t->dirty)
                continue;
            SDL_zero(from);
            SDL_zero(to);
            from.transfer_buffer = transfer;
            from.offset = offset;
            to.texture = t->gpu;
            to.w = t->width;
            to.h = t->height;
            to.d = 1;
            SDL_UploadToGPUTexture(copy, &from, &to, false);
            offset += t->width * t->height * 4;
            t->dirty = 0;
        }
        SDL_EndGPUCopyPass(copy);
    }

    if (drawCount || clearColor || clearDepth) {
        SDL_GPUColorTargetInfo color;
        SDL_GPUDepthStencilTargetInfo depthTarget;
        SDL_GPURenderPass *pass;
        SDL_GPUBufferBinding binding;
        float screen[4];
        u32 boundKey = PK_COUNT;
        Sdl3Texture *boundTexture = 0;
        int boundLinear = -1;
        const Sdl3PsParams *boundPs = 0;

        SDL_zero(color);
        color.texture = frames[current];
        color.load_op = clearColor ? SDL_GPU_LOADOP_CLEAR : SDL_GPU_LOADOP_LOAD;
        color.clear_color = clearColorValue;
        color.store_op = SDL_GPU_STOREOP_STORE;
        SDL_zero(depthTarget);
        depthTarget.texture = depth;
        depthTarget.load_op = clearDepth ? SDL_GPU_LOADOP_CLEAR : SDL_GPU_LOADOP_LOAD;
        depthTarget.clear_depth = clearDepthValue;
        depthTarget.store_op = SDL_GPU_STOREOP_STORE;
        depthTarget.stencil_load_op = SDL_GPU_LOADOP_DONT_CARE;
        depthTarget.stencil_store_op = SDL_GPU_STOREOP_DONT_CARE;
        pass = SDL_BeginGPURenderPass(cmd, &color, 1, &depthTarget);
        if (drawCount) {
            screen[0] = 2.0f / width;
            screen[1] = 2.0f / height;
            screen[2] = screen[3] = 0.0f;
            SDL_PushGPUVertexUniformData(cmd, 0, screen, sizeof(screen));
            binding.buffer = vertexBuffer;
            binding.offset = 0;
            SDL_BindGPUVertexBuffers(pass, 0, &binding, 1);
        }
        for (i = 0; i < drawCount; i++) {
            const Sdl3Draw *d = &draws[i];
            if (d->pipelineKey != boundKey) {
                SDL_GPUGraphicsPipeline *pipeline = Pipeline(d->pipelineKey);
                if (!pipeline)
                    continue;
                SDL_BindGPUGraphicsPipeline(pass, pipeline);
                boundKey = d->pipelineKey;
            }
            if (d->texture != boundTexture || d->linear != boundLinear) {
                SDL_GPUTextureSamplerBinding sampler;
                sampler.texture = d->texture->gpu;
                sampler.sampler = d->linear ? linearSampler : pointSampler;
                SDL_BindGPUFragmentSamplers(pass, 0, &sampler, 1);
                boundTexture = d->texture;
                boundLinear = d->linear;
            }
            if (!boundPs || SDL_memcmp(boundPs, &d->ps, sizeof(d->ps))) {
                SDL_PushGPUFragmentUniformData(cmd, 0, &d->ps, sizeof(d->ps));
                boundPs = &d->ps;
            }
            SDL_DrawGPUPrimitives(pass, d->vertexCount, 1, d->firstVertex, 0);
        }
        SDL_EndGPURenderPass(pass);
    }
    frameDraws += drawCount;
    frameVertices += vertexCount;
    vertexCount = drawCount = 0;
    clearColor = clearDepth = 0;
    BeginBatch();
}

/* the frame into the window (scaled, aspect kept), submitted; then the other frame is drawn into (a flip) */
long Sdl3RenderDevice::Present()
{
    SDL_GPUCommandBuffer *cmd;
    SDL_GPUTexture *swapchain = 0;
    Uint32 swapWidth = 0, swapHeight = 0;
    const char *presentTest = SDL_getenv("SDW_PRESENT_TEST");
    static int s_swapchainWarning;
    static int s_presentTestLogged;
    static unsigned s_presentedFrames;
    static int s_drawReported;
    static unsigned s_earlyDrawFrames, s_earlyVertices, s_earlyBatches;
    static int s_greenClearLogged;

    if (presentTest && presentTest[0] == '3' && presentTest[1] == 0 && !s_greenClearLogged++)
        SDL_Log("SheepD3D: green-clear test active; game draws should appear over a green background");

    s_presentedFrames++;
    if (s_presentedFrames <= 120) {
        if (drawCount)
            s_earlyDrawFrames++;
        s_earlyVertices += vertexCount;
        s_earlyBatches += drawCount;
    }
    if (drawCount && !s_drawReported) {
        SDL_Log("SheepD3D: first queued GPU draws: %u vertices in %u batches", (unsigned)vertexCount,
                (unsigned)drawCount);
        s_drawReported = 1;
    } else if (s_presentedFrames == 120 && !s_drawReported) {
        SDL_Log("SheepD3D: no GPU draw batches queued in the first 120 frames");
    }
    if (s_presentedFrames == 120)
        SDL_Log("SheepD3D: first 120 frames: %u draw frames, %u vertices, %u batches", s_earlyDrawFrames,
                s_earlyVertices, s_earlyBatches);

    PerfOverlay_PresentBegin(frameDraws + drawCount, frameVertices + vertexCount);
    frameDraws = frameVertices = 0;
    if (PerfOverlay_Enabled())
        DrawPerfOverlay();
    Flush();
    frameDraws = frameVertices = 0; /* the overlay's own draws are not counted */
    if (!(cmd = Commands())) {
        PerfOverlay_PresentEnd();
        return SDL3_E_FAIL;
    }
    if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmd, window, &swapchain, &swapWidth, &swapHeight)) {
        if (!s_swapchainWarning++)
            SDL_Log("SheepD3D: swapchain acquisition failed (%s)", SDL_GetError());
    } else if (swapchain && swapWidth && swapHeight) {
        if (presentTest && presentTest[0] == '1' && presentTest[1] == 0) {
            SDL_GPUColorTargetInfo color;
            SDL_GPURenderPass *pass;
            SDL_zero(color);
            color.texture = swapchain;
            color.load_op = SDL_GPU_LOADOP_CLEAR;
            color.clear_color.g = 1.0f;
            color.clear_color.a = 1.0f;
            color.store_op = SDL_GPU_STOREOP_STORE;
            pass = SDL_BeginGPURenderPass(cmd, &color, 1, 0);
            if (pass) {
                SDL_EndGPURenderPass(pass);
                if (!s_presentTestLogged++)
                    SDL_Log("SheepD3D: present test active; swapchain should be bright green");
            } else if (!s_swapchainWarning++) {
                SDL_Log("SheepD3D: present test render pass failed (%s)", SDL_GetError());
            }
        } else {
            SDL_GPUBlitInfo blit;
            float scale = SDL_min((float)swapWidth / width, (float)swapHeight / height);
            Uint32 w = (Uint32)(width * scale), h = (Uint32)(height * scale);
            if (presentTest && presentTest[0] == '2' && presentTest[1] == 0) {
                SDL_GPUColorTargetInfo color;
                SDL_GPURenderPass *pass;
                SDL_zero(color);
                color.texture = frames[current];
                color.load_op = SDL_GPU_LOADOP_CLEAR;
                color.clear_color.g = 1.0f;
                color.clear_color.a = 1.0f;
                color.store_op = SDL_GPU_STOREOP_STORE;
                pass = SDL_BeginGPURenderPass(cmd, &color, 1, 0);
                if (pass) {
                    SDL_EndGPURenderPass(pass);
                    if (!s_presentTestLogged++)
                        SDL_Log("SheepD3D: offscreen blit test active; window should be bright green");
                } else if (!s_swapchainWarning++) {
                    SDL_Log("SheepD3D: offscreen test render pass failed (%s)", SDL_GetError());
                }
            }
            SDL_zero(blit);
            blit.source.texture = frames[current];
            blit.source.w = width;
            blit.source.h = height;
            blit.destination.texture = swapchain;
            blit.destination.x = (swapWidth - w) / 2;
            blit.destination.y = (swapHeight - h) / 2;
            blit.destination.w = w;
            blit.destination.h = h;
            blit.load_op = SDL_GPU_LOADOP_CLEAR; /* black bars */
            blit.clear_color.a = 1.0f;
            blit.filter = SDL_GPU_FILTER_LINEAR;
            SDL_BlitGPUTexture(cmd, &blit);
        }
    } else if (!s_swapchainWarning++) {
        SDL_Log("SheepD3D: swapchain unavailable (texture=%p, size=%ux%u)", (void *)swapchain,
                (unsigned)swapWidth, (unsigned)swapHeight);
    }
    if (!SDL_SubmitGPUCommandBuffer(cmd) && !s_swapchainWarning++)
        SDL_Log("SheepD3D: submitting frame failed (%s)", SDL_GetError());
    commands = 0;
    current ^= 1;
    PerfOverlay_PresentEnd();
    return 0;
}

/* The performance overlay (src/render/perf_overlay.h): its lines in the top-left corner, white on a translucent black
 * box, drawn last into the frame with ordinary pre-transformed draws (and the game's state put back after). */
void Sdl3RenderDevice::DrawPerfOverlay()
{
    struct OverlayVertex {
        float x, y, z, rhw;
        u32 diffuse;
        float u, v;
    };
    static OverlayVertex quads[PERF_OVERLAY_MAX_LINES * PERF_OVERLAY_LINE_SIZE * 6];
    char lines[PERF_OVERLAY_MAX_LINES][PERF_OVERLAY_LINE_SIZE];
    const u32 fvf = RD_FVF_XYZRHW | RD_FVF_DIFFUSE | RD_FVF_TEX1;
    const int cellW = PERF_FONT_WIDTH + 1, cellH = PERF_FONT_HEIGHT + 3; /* a glyph and its spacing, in font pixels */
    int count = PerfOverlay_Lines(lines), longest = 0, glyphs = PerfOverlay_GlyphCount(), i, n = 0;
    float scale = (float)SDL_max(1u, height / 360), margin = 4.0f * scale, pad = 3.0f * scale;
    float atlasW = (float)(glyphs * cellW);
    Sdl3State saved = state;

    if (!count)
        return;
    if (!font.gpu) {
        /* the atlas: every glyph side by side in one row, white, its alpha the glyph (4444) */
        SDL_GPUTextureCreateInfo tex;
        int g, row, col;
        font.width = (u32)(glyphs * cellW);
        font.height = PERF_FONT_HEIGHT + 1;
        font.format = TEXFMT_ARGB4444;
        font.pixels = (u16 *)SDL_calloc(font.width * font.height, 2);
        if (!font.pixels)
            return;
        for (g = 0; g < glyphs; g++)
            for (row = 0; row < PERF_FONT_HEIGHT; row++)
                for (col = 0; col < PERF_FONT_WIDTH; col++)
                    if (PerfOverlay_GlyphRows(g)[row] & (0x10 >> col))
                        font.pixels[row * font.width + g * cellW + col] = 0xffff;
        SDL_zero(tex);
        tex.type = SDL_GPU_TEXTURETYPE_2D;
        tex.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
        tex.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
        tex.width = font.width;
        tex.height = font.height;
        tex.layer_count_or_depth = 1;
        tex.num_levels = 1;
        if (!(font.gpu = SDL_CreateGPUTexture(gpu, &tex)))
            return;
        font.dirty = 1;
        TrackTexture(&font);
    }
    for (i = 0; i < count; i++)
        longest = SDL_max(longest, (int)SDL_strlen(lines[i]));

    /* the box: untextured, the diffuse alpha blends it */
    state.blend = 1;
    state.srcBlend = D3D_BLEND_SRCALPHA;
    state.dstBlend = D3D_BLEND_INVSRCALPHA;
    state.alphaTest = state.zEnable = state.zWrite = state.specular = state.fog = state.linear = 0;
    state.cull = D3D_CULL_NONE;
    state.textured = 0;
    state.texture = 0;
    {
        float x0 = margin, y0 = margin;
        float x1 = x0 + 2 * pad + (longest * cellW - 1) * scale, y1 = y0 + 2 * pad + (count * cellH - 3) * scale;
        OverlayVertex box[6] = {{x0, y0, 0, 1, 0xa0000000, 0, 0}, {x1, y0, 0, 1, 0xa0000000, 0, 0},
                                {x0, y1, 0, 1, 0xa0000000, 0, 0}, {x1, y0, 0, 1, 0xa0000000, 0, 0},
                                {x1, y1, 0, 1, 0xa0000000, 0, 0}, {x0, y1, 0, 1, 0xa0000000, 0, 0}};
        DrawPrimitive(RD_PRIM_TRIANGLELIST, fvf, box, 6, 0);
    }

    /* the text: one quad per glyph, the texture's alpha cuts it out */
    for (i = 0; i < count; i++) {
        const char *c;
        float x = margin + pad, y = margin + pad + i * cellH * scale;
        for (c = lines[i]; *c; c++, x += cellW * scale) {
            int g = PerfOverlay_GlyphIndex(*c);
            float x1 = x + PERF_FONT_WIDTH * scale, y1 = y + PERF_FONT_HEIGHT * scale;
            float u0 = g * cellW / atlasW, u1 = (g * cellW + PERF_FONT_WIDTH) / atlasW;
            float v1 = (float)PERF_FONT_HEIGHT / font.height;
            OverlayVertex *q = quads + n;
            if (g < 0)
                continue;
            q[0] = {x, y, 0, 1, 0xffffffff, u0, 0};
            q[1] = {x1, y, 0, 1, 0xffffffff, u1, 0};
            q[2] = {x, y1, 0, 1, 0xffffffff, u0, v1};
            q[3] = {x1, y, 0, 1, 0xffffffff, u1, 0};
            q[4] = {x1, y1, 0, 1, 0xffffffff, u1, v1};
            q[5] = {x, y1, 0, 1, 0xffffffff, u0, v1};
            n += 6;
        }
    }
    state.textured = 1;
    state.texture = &font;
    if (n)
        DrawPrimitive(RD_PRIM_TRIANGLELIST, fvf, quads, (u32)n, 0);
    state = saved;
}

/* ---- textures ---- */
s32 Sdl3RenderDevice::CreateTexture(u32 w, u32 h, u32 format, RdTexture **out, u32 *outWidth, u32 *outHeight)
{
    Sdl3Texture *t;
    SDL_GPUTextureCreateInfo info;
    *out = 0;
    if (format != TEXFMT_RGB565 && format != TEXFMT_ARGB1555 && format != TEXFMT_ARGB4444 && format != TEXFMT_RGBA8)
        return TEXRES_NO_PIXEL_FORMAT;
    if (!w)
        w = 1;
    if (!h)
        h = 1;
    *outWidth = w;
    *outHeight = h;
    t = (Sdl3Texture *)SDL_calloc(1, sizeof(Sdl3Texture));
    if (!t || !(t->pixels = (u16 *)SDL_calloc(w * h, Sdl3_TexelBytes(format)))) {
        SDL_free(t);
        return TEXRES_OUT_OF_MEMORY;
    }
    SDL_zero(info);
    info.type = SDL_GPU_TEXTURETYPE_2D;
    info.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    info.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
    info.width = w;
    info.height = h;
    info.layer_count_or_depth = 1;
    info.num_levels = 1;
    if (!(t->gpu = SDL_CreateGPUTexture(gpu, &info)) || !TrackTexture(t)) {
        if (t->gpu)
            SDL_ReleaseGPUTexture(gpu, t->gpu);
        SDL_free(t->pixels);
        SDL_free(t);
        return TEXRES_OUT_OF_MEMORY;
    }
    t->width = w;
    t->height = h;
    t->format = format;
    t->dirty = 1;
    *out = (RdTexture *)t;
    return TEXRES_OK;
}

long Sdl3RenderDevice::LockTexture(RdTexture *texture, u32 access, RdLockedRect *out)
{
    Sdl3Texture *t = (Sdl3Texture *)texture;
    if (!t || !t->pixels)
        return SDL3_E_FAIL; /* a frame capture lives on the GPU only */
    if (access & RD_TEXLOCK_WRITE) {
        WaitUntilUnused(t); /* the batch drew with the texels as they are now */
        t->lockedToWrite = 1;
    }
    out->pixels = t->pixels;
    out->pitch = (s32)(t->width * Sdl3_TexelBytes(t->format));
    out->width = t->width;
    out->height = t->height;
    return 0;
}

long Sdl3RenderDevice::UnlockTexture(RdTexture *texture)
{
    Sdl3Texture *t = (Sdl3Texture *)texture;
    if (t && t->lockedToWrite) {
        t->lockedToWrite = 0;
        t->dirty = 1;
    }
    return 0;
}

long Sdl3RenderDevice::CopyTexture(RdTexture *dstTexture, u32 x, u32 y, RdTexture *srcTexture, RdRect *srcRect)
{
    Sdl3Texture *dst = (Sdl3Texture *)dstTexture, *src = (Sdl3Texture *)srcTexture;
    RdRect r;
    s32 row;
    if (!dst || !src || !dst->pixels || !src->pixels || Sdl3_TexelBytes(dst->format) != Sdl3_TexelBytes(src->format))
        return SDL3_E_FAIL;
    if (srcRect)
        r = *srcRect;
    else {
        r.left = r.top = 0;
        r.right = (s32)src->width;
        r.bottom = (s32)src->height;
    }
    if (r.left < 0 || r.top < 0 || r.right > (s32)src->width || r.bottom > (s32)src->height || r.right <= r.left ||
        r.bottom <= r.top || x + (u32)(r.right - r.left) > dst->width || y + (u32)(r.bottom - r.top) > dst->height)
        return SDL3_E_FAIL; /* BltFast's DDERR_INVALIDRECT */
    WaitUntilUnused(dst);
    {
        u32 bytes = Sdl3_TexelBytes(src->format);
        u8 *to = (u8 *)dst->pixels, *from = (u8 *)src->pixels;
        for (row = r.top; row < r.bottom; row++)
            SDL_memmove(to + ((y + (u32)(row - r.top)) * dst->width + x) * bytes,
                        from + ((u32)row * src->width + (u32)r.left) * bytes, (size_t)(r.right - r.left) * bytes);
    }
    dst->dirty = 1;
    return 0;
}

void Sdl3RenderDevice::ReleaseTexture(RdTexture *texture)
{
    Sdl3Texture *t = (Sdl3Texture *)texture;
    if (!t)
        return;
    WaitUntilUnused(t); /* the recorded draws still use it */
    if (state.texture == t)
        state.texture = 0;
    UntrackTexture(t);
    SDL_ReleaseGPUTexture(gpu, t->gpu); /* SDL keeps it until the submitted commands are done with it */
    SDL_free(t->pixels);
    SDL_free(t);
}

long Sdl3RenderDevice::CreateFrameCaptureTexture(u32 w, u32 h, RdTexture **out, u32 *outWidth, u32 *outHeight)
{
    Sdl3Texture *t;
    SDL_GPUTextureCreateInfo info;
    *out = 0;
    if (!w || !h || w > width || h > height)
        return SDL3_E_FAIL;
    if (!(t = (Sdl3Texture *)SDL_calloc(1, sizeof(Sdl3Texture))))
        return SDL3_E_FAIL;
    SDL_zero(info);
    info.type = SDL_GPU_TEXTURETYPE_2D;
    info.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM; /* the frames' format, for the copy */
    info.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
    info.width = w;
    info.height = h;
    info.layer_count_or_depth = 1;
    info.num_levels = 1;
    if (!(t->gpu = SDL_CreateGPUTexture(gpu, &info))) {
        SDL_free(t);
        return SDL3_E_FAIL;
    }
    t->width = *outWidth = w;
    t->height = *outHeight = h;
    *out = (RdTexture *)t;
    return 0;
}

long Sdl3RenderDevice::CopyFrameToTexture(RdTexture *dstTexture, RdRect *dstRect, int fromShown)
{
    Sdl3Texture *dst = (Sdl3Texture *)dstTexture;
    SDL_GPUCommandBuffer *cmd;
    SDL_GPUCopyPass *copy;
    SDL_GPUTextureLocation from, to;
    u32 w, h;
    if (!dst || !dstRect)
        return SDL3_E_FAIL;
    WaitUntilUnused(dst);
    Flush(); /* the frame so far */
    if (!(cmd = Commands()))
        return SDL3_E_FAIL;
    /* the original stretches the whole frame into dstRect; the capture is the frame's size, so this is a copy */
    w = SDL_min((u32)(dstRect->right - dstRect->left), SDL_min(width, dst->width - (u32)dstRect->left));
    h = SDL_min((u32)(dstRect->bottom - dstRect->top), SDL_min(height, dst->height - (u32)dstRect->top));
    SDL_zero(from);
    SDL_zero(to);
    from.texture = frames[fromShown ? current ^ 1 : current];
    to.texture = dst->gpu;
    to.x = (u32)dstRect->left;
    to.y = (u32)dstRect->top;
    copy = SDL_BeginGPUCopyPass(cmd);
    SDL_CopyGPUTextureToTexture(copy, &from, &to, w, h, 1, false);
    SDL_EndGPUCopyPass(copy);
    return 0;
}

/* ---- the backend ---- */
static int s_sdl3VideoInit;

static int Sdl3_InitVideo()
{
    if (!s_sdl3VideoInit) {
        if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) /* started by Platform_Init already: this counts a reference */
            return 0;
        s_sdl3VideoInit = 1;
    }
    return 1;
}

static SDL_GPUShaderFormat Sdl3_ShaderFormats()
{
    return SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXBC | SDL_GPU_SHADERFORMAT_MSL;
}

class Sdl3RenderBackend : public RenderBackend {
public:
    Sdl3RenderBackend(const char *driver, const char *name) : driver(driver), name(name) {}
    const char *Name() const { return name; }
    int PrepareDisplay(D3DApp *app)
    {
        (void)app;
        if (!Sdl3_InitVideo() || !SDL_GPUSupportsShaderFormats(Sdl3_ShaderFormats(), driver)) {
            SDL_Log("SheepD3D: %s: %s", driver, SDL_GetError());
            return 0;
        }
        return 1;
    }
    /* a normal window at the render size, resizable (the frame is scaled into it); Vulkan and Metal need flags */
    unsigned WindowFlags() const
    {
        unsigned flags = PLATFORM_WINDOW_DECORATED;
        if (SDL_strcmp(driver, "vulkan") == 0)
            flags |= PLATFORM_WINDOW_VULKAN;
        else if (SDL_strcmp(driver, "metal") == 0)
            flags |= PLATFORM_WINDOW_METAL;
        return flags;
    }
    void WindowSize(int *width, int *height) const
    {
        *width = SDL3_RENDER_WIDTH;
        *height = SDL3_RENDER_HEIGHT;
    }

    RenderDevice *CreateDevice(D3DApp *app, long *result)
    {
        SDL_GPUDevice *gpu;
        SDL_Window *window = Platform_Window();
        Sdl3RenderDevice *device;
        const u32 width = SDL3_RENDER_WIDTH, height = SDL3_RENDER_HEIGHT;

        *result = SDL3_E_FAIL;
        if (!window || !Sdl3_InitVideo())
            return 0;
        if (!(gpu = SDL_CreateGPUDevice(Sdl3_ShaderFormats(), false, driver))) {
            SDL_Log("SheepD3D: SDL_CreateGPUDevice(%s): %s", driver, SDL_GetError());
            return 0;
        }
        SDL_Log("SheepD3D: using SDL GPU driver %s", SDL_GetGPUDeviceDriver(gpu));
        if (!SDL_ClaimWindowForGPUDevice(gpu, window)) {
            SDL_Log("SheepD3D: SDL_ClaimWindowForGPUDevice: %s", SDL_GetError());
            SDL_DestroyGPUDevice(gpu);
            return 0;
        }
        device = new Sdl3RenderDevice(gpu, window, width, height);
        if (!device->Init()) {
            SDL_Log("SheepD3D: %s: %s", device->Name(), SDL_GetError());
            delete device;
            return 0;
        }
        *result = D3DApp_StartWithoutDirectDraw(app, width, height);
        if (*result < 0) {
            delete device;
            return 0;
        }
        return device;
    }

private:
    const char *driver, *name;
};

RenderBackend *Sdl3_CreateRenderBackend(const char *driver, const char *name)
{
    return new Sdl3RenderBackend(driver, name);
}

int Sdl3_DriverAvailable(const char *driver)
{
    return Sdl3_InitVideo() && SDL_GPUSupportsShaderFormats(Sdl3_ShaderFormats(), driver);
}
