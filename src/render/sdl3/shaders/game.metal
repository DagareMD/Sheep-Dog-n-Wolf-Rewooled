// The SDL3 GPU renderer's shaders for Metal: game.hlsl, by hand (see there). Not tested yet: no Apple build exists.
#include <metal_stdlib>
using namespace metal;

struct VSParams {
    float4 screen; // x: 2 / width, y: 2 / height
};

struct VSIn {
    float4 pos [[attribute(0)]];      // x, y, z, rhw
    float4 diffuse [[attribute(1)]];  // a D3DCOLOR's bytes: b, g, r, a
    float4 specular [[attribute(2)]]; // the same; its alpha is the fog factor
    float2 uv [[attribute(3)]];
};

struct VSOut {
    float4 pos [[position]];
    float4 diffuse;
    float4 specular;
    float2 uv;
};

vertex VSOut VSMain(VSIn i [[stage_in]], constant VSParams &p [[buffer(0)]])
{
    VSOut o;
    float w = 1.0 / (abs(i.pos.w) > 1e-30 ? i.pos.w : 1e-30); // signed: behind-the-eye vertices are clipped

    float x = (i.pos.x + 0.5) * p.screen.x - 1.0;
    float y = 1.0 - (i.pos.y + 0.5) * p.screen.y;
    o.pos = float4(x * w, y * w, i.pos.z * w, w);
    o.diffuse = i.diffuse.zyxw;
    o.specular = i.specular.zyxw;
    o.uv = i.uv;
    return o;
}

struct PSParams {
    float4 fogColor;
    float4 mode; // x: 0 diffuse, 1 MODULATE, 2 MODULATE2X; y: alpha test, z: fog, w: specular (0 or 1)
};

fragment float4 PSMain(VSOut i [[stage_in]], constant PSParams &p [[buffer(0)]], texture2d<float> tex [[texture(0)]],
                       sampler smp [[sampler(0)]])
{
    float4 c = i.diffuse;
    if (p.mode.x > 0.5) {
        // Saturate the texture stage before specular and fog; keep the texture's alpha.
        float4 t = tex.sample(smp, i.uv);
        c = float4(saturate(t.rgb * i.diffuse.rgb * p.mode.x), t.a);
    }
    if (p.mode.w > 0.5)
        c.rgb = saturate(c.rgb + i.specular.rgb);
    if (p.mode.z > 0.5)
        c.rgb = mix(p.fogColor.rgb, c.rgb, i.specular.a);
    if (p.mode.y > 0.5 && c.a * 255.0 > 8.5)
        discard_fragment();
    return c;
}
