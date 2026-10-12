/*
 * C++; every function keeps C linkage (extern "C") so the names stay what C callers link against.
 *
 * Many of these read like a C port of the PlayStation's libgte: Mat34s is a GTE MATRIX (s16 m[3][3] in 4.12, s32 t[3]),
 * Vec4s/Vec4i are SVECTOR/VECTOR (with their unused pad word), and the Mat34s_Transform* family returns its output
 * pointer the way ApplyMatrix/ApplyMatrixSV/ApplyMatrixLV do. That is a reading of the shapes, not a proof of origin.
 */
#include "sdw_types.h"
#define SDW_MEMBERS_Mat44 Mat44(); /* Mat44_Ctor: empty, 52 callers                    */
#include "sdw_classes.h"

/* explicit spelling of the hidden result pointer. Keep the unnamed, constructor-free result temporary
 * and subsequent block copy at the call site while agreeing with the declaration of Mat44_Mul's definition. */
#include "mat44.h"
struct MatrixReturnStorage {
    float m[4][4];
    MatrixReturnStorage() {}
};

#include "../sdk/crt.h"
extern "C" {

/* float to 4.12, sign-symmetric: floor(|f|) << 12 plus the truncated fraction * 4096, negated for f < 0,
 * cut to 16 bits. Local names are chosen for their slots (frac -8, ip -0xc, fracBits -0xe; the second block's copies
 * below the first's). */
s16 Math_FloatToFixed12_s16(float f)
{
    s16 result;
    if (f >= 0.0f) {
        s16 fracBits;
        float ip;
        float frac;
        ip = (float)floor(f);
        fracBits = (s16)((frac = f - ip) * 4096.0f);
        result = ((s16)ip << 12) + fracBits;
    } else {
        s16 fracBits;
        float ip;
        float frac;
        f = -f;
        ip = (float)floor(f);
        fracBits = (s16)((frac = f - ip) * 4096.0f);
        result = ((s16)ip << 12) + fracBits;
        result = -result;
    }
    return result;
}

/* as Math_FloatToFixed12_s16 with an s32 result (slots frac -8, fi -0xc, ip -0x10). */
s32 Math_FloatToFixed12_s32(float f)
{
    s32 result;
    if (f >= 0.0f) {
        float ip;
        s32 fi;
        float frac;
        ip = (float)floor(f);
        fi = (s32)((frac = f - ip) * 4096.0f);
        result = ((s32)ip << 12) + fi;
    } else {
        float ip;
        s32 fi;
        float frac;
        f = -f;
        ip = (float)floor(f);
        fi = (s32)((frac = f - ip) * 4096.0f);
        result = ((s32)ip << 12) + fi;
        result = -result;
    }
    return result;
}

/* 4.12 fixed to float: the integer part plus the 12-bit fraction / 4096, left in ST0. */
float Math_Fixed12ToFloat_s16(s16 v)
{
    s16 frac = v & 0xfff;
    s16 whole = (v - frac) >> 12;
    float result;
    return result = whole + frac / 4096.0f;
}

/* the s32 twin. */
float Math_Fixed12ToFloat_s32(s32 v)
{
    s32 frac = v & 0xfff;
    s32 whole = (v - frac) >> 12;
    float f;
    return f = whole + frac / 4096.0f;
}

/* float to 6.10, as Math_FloatToFixed12_s16 with 1024. No callers. */
s16 Math_FloatToFixed10_s16(float f)
{
    s16 result;
    if (f >= 0.0f) {
        s16 fracBits;
        float ip;
        float frac;
        ip = (float)floor(f);
        fracBits = (s16)((frac = f - ip) * 1024.0f);
        result = ((s16)ip << 10) + fracBits;
    } else {
        s16 fracBits;
        float ip;
        float frac;
        f = -f;
        ip = (float)floor(f);
        fracBits = (s16)((frac = f - ip) * 1024.0f);
        result = ((s16)ip << 10) + fracBits;
        result = -result;
    }
    return result;
}

/* float to 6.10, s32 result. BUG in the original: the negative branch ends `result - result` instead of negating, so
 * every negative input returns 0. No callers. */
s32 Math_FloatToFixed10_s32(float f)
{
    s32 result;
    if (f >= 0.0f) {
        float ip;
        s32 fi;
        float frac;
        ip = (float)floor(f);
        fi = (s32)((frac = f - ip) * 1024.0f);
        result = ((s32)ip << 10) + fi;
    } else {
        float ip;
        s32 fi;
        float frac;
        f = -f;
        ip = (float)floor(f);
        fi = (s32)((frac = f - ip) * 1024.0f);
        result = ((s32)ip << 10) + fi;
        result = result - result;
    }
    return result;
}

/* 6.10 fixed to float: the integer part plus the 10-bit fraction / 1024. The result is left in ST0 by an assignment to a
 * local. Slots: result -4, whole -6, frac -8 (buckets 0, 1, 7); the s32 twin below has the float in the middle, so its
 * name differs. */
float Math_Fixed10ToFloat_s16(s16 v)
{
    s16 frac = v & 0x3ff;
    s16 whole = (v - frac) >> 10;
    float result;
    return result = whole + frac / 1024.0f;
}

/* the s32 twin of Math_Fixed10ToFloat_s16. No callers. */
float Math_Fixed10ToFloat_s32(s32 v)
{
    s32 frac = v & 0x3ff;
    s32 whole = (v - frac) >> 10;
    float f;
    return f = whole + frac / 1024.0f;
}

/* 12-bit angle (4096 per turn) to radians. */
float Math_Angle4096ToRadians_2(s16 angle)
{
    float r;
    return r = angle * 2.0f * 3.14159265358979f / 4096.0f;
}

/* radians to the 12-bit angle. The float arrives on the stack like any argument; the function scales it itself (constants
 * and) and truncates with __ftol. Callers mask the result to 0..4095. */
s16 Math_RadiansToAngle4096(float radians)
{
    s16 angle = (s16)(radians * 4096.0f / 6.283185307179586f);
    return angle;
}

/* ColorFix's expansion for untextured faces and the level clear/fog colour. Keep the literal 1.03f,
 * truncation and saturation; the upper byte is unchanged. */
u32 Color_ExpandPs1(u32 c)
{
    u32 out = c & 0xff000000;
    for (int shift = 0; shift < 24; shift += 8) {
        u32 ch = (u32)((c >> shift & 0xff) * 1.03f);
        out |= (ch > 0xff ? 0xff : ch) << shift;
    }
    return out;
}

/* swaps the red and blue bytes; the top byte comes out 0. */
u32 Color_RgbToBgr(u32 c)
{
    return ((c >> 16) & 0xff) + ((c << 16) & 0xff0000) + (c & 0xff00);
}

/* swaps red and blue and halves every channel. */
u32 Color_RgbToBgrHalved(u32 c)
{
    return ((c >> 17) & 0xff) + ((c << 15) & 0xff0000) + ((c >> 1) & 0xff00);
}

/* the inverse of Color_RgbToBgrHalved: swaps and doubles (exact for channels <= 0x7f). */
u32 Color_BgrHalvedToRgb(u32 c)
{
    return ((c >> 15) & 0xff) + ((c << 17) & 0xff0000) + ((c << 1) & 0xff00);
}

/* out = v / |v| in 4.12, through float. A zero vector divides by zero. The length is the top local (-4), above x, y, z. */
void Vec3i_Normalize(const Vec3i *v, Vec3i *out)
{
    float length;
    float x;
    float y;
    float z;
    x = (float)v->x;
    y = (float)v->y;
    z = (float)v->z;
    length = (float)sqrt(x * x + y * y + z * z);
    out->x = Math_FloatToFixed12_s32(x / length);
    out->y = Math_FloatToFixed12_s32(y / length);
    out->z = Math_FloatToFixed12_s32(z / length);
}

/* as Vec3i_Normalize for Vec3s. */
void Vec3s_Normalize(const Vec3s *v, Vec3s *out)
{
    float length;
    float x;
    float y;
    float z;
    x = (float)v->x;
    y = (float)v->y;
    z = (float)v->z;
    length = (float)sqrt(x * x + y * y + z * z);
    out->x = Math_FloatToFixed12_s16(x / length);
    out->y = Math_FloatToFixed12_s16(y / length);
    out->z = Math_FloatToFixed12_s16(z / length);
}

/* integer dot product computed in floating point and saturated to the s32 range; no 4.12 shift. */
s32 Vec3s_Dot(const Vec3s *a, const Vec3s *b)
{
    double d = (double)a->x * b->x + (double)a->y * b->y + (double)a->z * b->z;
    if (d > 2147483647.0)
        return 0x7fffffff;
    if (d < -2147483648.0)
        return 0x80000000;
    return (s32)d;
}

/* cross product of 4.12 vectors, through float. The original interleaves the slots b.x -4, a.x -8, b.y -0xc, a.y -0x10,
 * b.z -0x14, a.z -0x18. */
void Vec3i_Cross(const Vec3i *a, const Vec3i *b, Vec3i *out)
{
    float a_x, a_y, a_z, b_x, b_y, b_z;
    a_x = Math_Fixed12ToFloat_s32(a->x);
    a_y = Math_Fixed12ToFloat_s32(a->y);
    a_z = Math_Fixed12ToFloat_s32(a->z);
    b_x = Math_Fixed12ToFloat_s32(b->x);
    b_y = Math_Fixed12ToFloat_s32(b->y);
    b_z = Math_Fixed12ToFloat_s32(b->z);
    out->x = Math_FloatToFixed12_s32(a_y * b_z - a_z * b_y);
    out->y = Math_FloatToFixed12_s32(a_z * b_x - a_x * b_z);
    out->z = Math_FloatToFixed12_s32(a_x * b_y - a_y * b_x);
}

s32 Vec3s_LengthSq(const Vec3s *v)
{
    return v->x * v->x + v->y * v->y + v->z * v->z;
}

void Mat34s_Identity(struct Mat34s *m)
{
    m->rot[0] = m->rot[4] = m->rot[8] = 4096;
    m->rot[3] = m->rot[6] = m->rot[1] = m->rot[7] = m->rot[2] = m->rot[5] = 0;
    m->trans[0] = m->trans[1] = m->trans[2] = 0;
}

/* rotation from 12-bit Euler angles through the float Mat44_SetRotYXZ, converted to 4.12 and transposed
 * into Mat34s order (rot[3*i+j] = m[4*j+i]); the translation is the float matrix's, truncated to s16 first. */
void Mat34s_FromEulerYXZ(const Vec3s *angles, Mat34s *out)
{
    Mat44 m;
    m.SetRotYXZ(Math_Angle4096ToRadians_2(angles->x), Math_Angle4096ToRadians_2(angles->y),
                Math_Angle4096ToRadians_2(angles->z));
    out->rot[0] = Math_FloatToFixed12_s16(m.m[0][0]);
    out->rot[1] = Math_FloatToFixed12_s16(m.m[1][0]);
    out->rot[2] = Math_FloatToFixed12_s16(m.m[2][0]);
    out->rot[3] = Math_FloatToFixed12_s16(m.m[0][1]);
    out->rot[4] = Math_FloatToFixed12_s16(m.m[1][1]);
    out->rot[5] = Math_FloatToFixed12_s16(m.m[2][1]);
    out->rot[6] = Math_FloatToFixed12_s16(m.m[0][2]);
    out->rot[7] = Math_FloatToFixed12_s16(m.m[1][2]);
    out->rot[8] = Math_FloatToFixed12_s16(m.m[2][2]);
    out->trans[0] = (s16)m.m[3][0];
    out->trans[1] = (s16)m.m[3][1];
    out->trans[2] = (s16)m.m[3][2];
}

/* as Mat34s_FromEulerYXZ with Mat44_SetRotXZY. */
void Mat34s_FromEulerXZY(const Vec3s *angles, Mat34s *out)
{
    Mat44 m;
    m.SetRotXZY(Math_Angle4096ToRadians_2(angles->x), Math_Angle4096ToRadians_2(angles->y),
                Math_Angle4096ToRadians_2(angles->z));
    out->rot[0] = Math_FloatToFixed12_s16(m.m[0][0]);
    out->rot[1] = Math_FloatToFixed12_s16(m.m[1][0]);
    out->rot[2] = Math_FloatToFixed12_s16(m.m[2][0]);
    out->rot[3] = Math_FloatToFixed12_s16(m.m[0][1]);
    out->rot[4] = Math_FloatToFixed12_s16(m.m[1][1]);
    out->rot[5] = Math_FloatToFixed12_s16(m.m[2][1]);
    out->rot[6] = Math_FloatToFixed12_s16(m.m[0][2]);
    out->rot[7] = Math_FloatToFixed12_s16(m.m[1][2]);
    out->rot[8] = Math_FloatToFixed12_s16(m.m[2][2]);
    out->trans[0] = (s16)m.m[3][0];
    out->trans[1] = (s16)m.m[3][1];
    out->trans[2] = (s16)m.m[3][2];
}

/* m = Scale(scale) * m, with the scale in 6.10 (1024 = 1.0), done in float and written back to 4.12. The rotation copy is
 * at -0x40 and constructed first, the scale matrix at -0x80; the product lands in the compiler's return temporary at
 * -0xc0 and is block-copied back (rep movsd, no operator= call). */
void Mat34s_ApplyScale(Mat34s *m, const Vec3s *scale)
{
    Mat44 mat;
    Mat44 sc;
    MatrixReturnStorage result;
    mat.SetIdentity();
    mat.m[0][0] = Math_Fixed12ToFloat_s16(m->rot[0]);
    mat.m[0][1] = Math_Fixed12ToFloat_s16(m->rot[3]);
    mat.m[0][2] = Math_Fixed12ToFloat_s16(m->rot[6]);
    mat.m[1][0] = Math_Fixed12ToFloat_s16(m->rot[1]);
    mat.m[1][1] = Math_Fixed12ToFloat_s16(m->rot[4]);
    mat.m[1][2] = Math_Fixed12ToFloat_s16(m->rot[7]);
    mat.m[2][0] = Math_Fixed12ToFloat_s16(m->rot[2]);
    mat.m[2][1] = Math_Fixed12ToFloat_s16(m->rot[5]);
    mat.m[2][2] = Math_Fixed12ToFloat_s16(m->rot[8]);
    mat.m[3][0] = (float)m->trans[0];
    mat.m[3][1] = (float)m->trans[1];
    mat.m[3][2] = (float)m->trans[2];
    sc.SetScale(Math_Fixed10ToFloat_s16(scale->x), Math_Fixed10ToFloat_s16(scale->y),
                Math_Fixed10ToFloat_s16(scale->z));
    /* The return buffer is constructor-free storage with a named address for Clang. */
    mat = *Mat44_Mul((Mat44 *)&result, &sc, &mat);
    m->rot[0] = Math_FloatToFixed12_s16(mat.m[0][0]);
    m->rot[1] = Math_FloatToFixed12_s16(mat.m[1][0]);
    m->rot[2] = Math_FloatToFixed12_s16(mat.m[2][0]);
    m->rot[3] = Math_FloatToFixed12_s16(mat.m[0][1]);
    m->rot[4] = Math_FloatToFixed12_s16(mat.m[1][1]);
    m->rot[5] = Math_FloatToFixed12_s16(mat.m[2][1]);
    m->rot[6] = Math_FloatToFixed12_s16(mat.m[0][2]);
    m->rot[7] = Math_FloatToFixed12_s16(mat.m[1][2]);
    m->rot[8] = Math_FloatToFixed12_s16(mat.m[2][2]);
    m->trans[0] = (s16)mat.m[3][0];
    m->trans[1] = (s16)mat.m[3][1];
    m->trans[2] = (s16)mat.m[3][2];
}

/* column `col` of the upper 3x3 of a float matrix (m[col], m[4+col], m[8+col]), truncated to s16 with no
 * 4.12 scaling; returned by value (6-byte struct through the hidden pointer). No callers. */
Vec3s Mat44_GetColumnVec3s(const Mat44 *m, s32 col)
{
    Vec3s v;
    v.x = (s16)m->m[0][col];
    v.y = (s16)m->m[1][col];
    v.z = (s16)m->m[2][col];
    return v;
}

/* as Mat44_GetColumnVec3s, truncated to s32. No callers. */
Vec3i Mat44_GetColumnVec3i(const Mat44 *m, s32 col)
{
    Vec3i v;
    v.x = (s32)m->m[0][col];
    v.y = (s32)m->m[1][col];
    v.z = (s32)m->m[2][col];
    return v;
}

/* out = rot * v + trans, each product shifted by 12 on its own (so the sum can differ from one shift by up
 * to 2). The result goes through a 16-byte local whose fourth word is never written: out->pad receives stack garbage. */
Vec4i *Mat34s_TransformVec3s_i(const Mat34s *m, const Vec3s *v, Vec4i *out)
{
    Vec4i r;
    r.x = (m->rot[0] * v->x >> 12) + (m->rot[1] * v->y >> 12) + (m->rot[2] * v->z >> 12) + m->trans[0];
    r.y = (m->rot[3] * v->x >> 12) + (m->rot[4] * v->y >> 12) + (m->rot[5] * v->z >> 12) + m->trans[1];
    r.z = (m->rot[6] * v->x >> 12) + (m->rot[7] * v->y >> 12) + (m->rot[8] * v->z >> 12) + m->trans[2];
    *out = r;
    return out;
}

/* as Mat34s_TransformVec3s_i with an s16 result, adding only the low 16 bits of each translation; writes
 * 8 bytes (out->pad is stack garbage). */
Vec4s *Mat34s_TransformVec3s(const Mat34s *m, const Vec3s *v, Vec4s *out)
{
    Vec4s r;
    r.x = (m->rot[0] * v->x >> 12) + (m->rot[1] * v->y >> 12) + (m->rot[2] * v->z >> 12) + (s16)m->trans[0];
    r.y = (m->rot[3] * v->x >> 12) + (m->rot[4] * v->y >> 12) + (m->rot[5] * v->z >> 12) + (s16)m->trans[1];
    r.z = (m->rot[6] * v->x >> 12) + (m->rot[7] * v->y >> 12) + (m->rot[8] * v->z >> 12) + (s16)m->trans[2];
    *out = r;
    return out;
}

/* as Mat34s_TransformVec3s_i for an s32 vector. */
Vec4i *Mat34s_TransformVec3i(const Mat34s *m, const Vec3i *v, Vec4i *out)
{
    Vec4i r;
    r.x = (m->rot[0] * v->x >> 12) + (m->rot[1] * v->y >> 12) + (m->rot[2] * v->z >> 12) + m->trans[0];
    r.y = (m->rot[3] * v->x >> 12) + (m->rot[4] * v->y >> 12) + (m->rot[5] * v->z >> 12) + m->trans[1];
    r.z = (m->rot[6] * v->x >> 12) + (m->rot[7] * v->y >> 12) + (m->rot[8] * v->z >> 12) + m->trans[2];
    *out = r;
    return out;
}

/* rotates by the TRANSPOSE of rot and then adds trans (so it is not the inverse transform). */
Vec4i *Mat34s_TransformTransposedVec3i(const Mat34s *m, const Vec3i *v, Vec4i *out)
{
    Vec4i r;
    r.x = (m->rot[0] * v->x >> 12) + (m->rot[3] * v->y >> 12) + (m->rot[6] * v->z >> 12) + m->trans[0];
    r.y = (m->rot[1] * v->x >> 12) + (m->rot[4] * v->y >> 12) + (m->rot[7] * v->z >> 12) + m->trans[1];
    r.z = (m->rot[2] * v->x >> 12) + (m->rot[5] * v->y >> 12) + (m->rot[8] * v->z >> 12) + m->trans[2];
    *out = r;
    return out;
}

} /* extern "C" */
