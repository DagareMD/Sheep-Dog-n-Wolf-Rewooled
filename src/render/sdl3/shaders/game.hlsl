/* The SDL3 GPU renderer's shaders (src/render/sdl3/): Direct3D 7's fixed-function pipeline, as far as the game uses it.
 *
 * Every vertex the game draws is pre-transformed (XYZRHW): x, y in pixels with Direct3D's pixel centres on whole
 * numbers, z in 0..1, rhw = 1/w. The vertex shader turns that back into clip space (with w, so that the texture and
 * colours are interpolated with perspective, as Direct3D does). The pixel shader is texture stage 0 (MODULATE,
 * MODULATE2X or the diffuse colour), the specular add, per-vertex fog (the specular alpha is the fog factor) and the
 * alpha test.
 *
 * Compiled by tools/sdl3_shaders.py into src/render/sdl3/sdl3_shaders.h (DXBC for Direct3D 12, SPIR-V for Vulkan);
 * game.metal is the same for Metal. The resource registers are SDL_CreateGPUShader's. */
#ifdef __spirv__
#define VK_BINDING(slot, set) [[vk::binding(slot, set)]]
#define VK_COMBINED [[vk::combinedImageSampler]]
#else
#define VK_BINDING(slot, set)
#define VK_COMBINED
#endif

VK_BINDING(0, 1) cbuffer VSParams : register(b0, space1)
{
    float4 screen; /* x: 2 / width, y: 2 / height */
};

struct VSIn {
    float4 pos : TEXCOORD0;      /* x, y, z, rhw */
    float4 diffuse : TEXCOORD1;  /* a D3DCOLOR's bytes: b, g, r, a */
    float4 specular : TEXCOORD2; /* the same; its alpha is the fog factor */
    float2 uv : TEXCOORD3;
};

struct VSOut {
    float4 diffuse : TEXCOORD0; /* r, g, b, a */
    float4 specular : TEXCOORD1;
    float2 uv : TEXCOORD2;
    float4 pos : SV_Position;
};

VSOut VSMain(VSIn i)
{
    VSOut o;
    /* w keeps its sign: the game submits triangles with vertices behind the eye (rhw < 0) and leaves them to the
     * clipper, which cuts them at the near plane as Direct3D does */
    float w = 1.0 / (abs(i.pos.w) > 1e-30 ? i.pos.w : 1e-30);
    /* + 0.5: Direct3D 7 puts a pixel's centre at whole coordinates, today's APIs half a pixel further */
    float x = (i.pos.x + 0.5) * screen.x - 1.0;
    float y = 1.0 - (i.pos.y + 0.5) * screen.y;
    o.pos = float4(x * w, y * w, i.pos.z * w, w);
    o.diffuse = i.diffuse.zyxw;
    o.specular = i.specular.zyxw;
    o.uv = i.uv;
    return o;
}

VK_COMBINED VK_BINDING(0, 2) Texture2D tex : register(t0, space2);
VK_COMBINED VK_BINDING(0, 2) SamplerState smp : register(s0, space2);

VK_BINDING(0, 3) cbuffer PSParams : register(b0, space3)
{
    float4 fogColor; /* r, g, b */
    float4 mode;     /* x: 0 diffuse, 1 MODULATE, 2 MODULATE2X; y: alpha test, z: fog, w: specular (0 or 1) */
};

float4 PSMain(VSOut i) : SV_Target0
{
    float4 c = i.diffuse;
    if (mode.x > 0.5) {
        /* Saturate the texture stage before specular and fog. ALPHAOP SELECTARG1 keeps the texture's alpha. */
        float4 t = tex.Sample(smp, i.uv);
        c = float4(saturate(t.rgb * i.diffuse.rgb * mode.x), t.a);
    }
    if (mode.w > 0.5)
        c.rgb = saturate(c.rgb + i.specular.rgb);
    if (mode.z > 0.5)
        c.rgb = lerp(fogColor.rgb, c.rgb, i.specular.a);
    /* ALPHAREF 8, ALPHAFUNC LESSEQUAL (the game's alpha is inverted: 0 is opaque) */
    if (mode.y > 0.5 && c.a * 255.0 > 8.5)
        discard;
    return c;
}
