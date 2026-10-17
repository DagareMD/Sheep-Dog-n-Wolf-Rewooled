/*
 * The launcher's Language Export (langexport.h): the PlayStation .DAV (LEVxx.DAV on the disc), as the PlayStation
 * executable's loader reads it, decoded into the PC's VDX7 layout, so that a PlayStation release's pictures and font are
 * read as a PC release's. The launcher's own copy of the game's reading of it.
 *
 * The PlayStation file (little-endian):
 *   +0x00 u32 checksum; "V2.6"; u32 0x14
 *   +0x14 u16 materialCount, textureCount, clutCount, pageCount
 *   +0x1c u32 file offsets: materials, textures, cluts, end of the tables (the pixel data follows), exported lists
 *   material {u16 texture, palette, flags}          the .WAR's texture ids are material indices, like the PC entries
 *   texture  {u16 x, w, y, h, page, mode}            x, w in 16-bit VRAM units of the page (4 texels at 4 bpp, mode 2;
 *                                                    2 at 8 bpp, mode 1; 1 at 15 bpp), y, h in rows
 *   clut     {u16 x, w, y, h, page, 0}               a palette: w colours from (x, y) of its page
 *   exported {u32 count; {u16 id; u16 n; u32 materialOffset[n]}}   the bitmaps the code asks for by id (DAV_IDI_*),
 *                                                    each a file offset into the material table
 *   pixels   after a u32 at the end of the tables: pageCount x {u16 x, w, y, h; w * h u16}, a VRAM rectangle each
 *   "END "
 * The coordinates of textures and palettes are relative to their page (the loader adds the page's VRAM position).
 * Colours are 15-bit BGR; 0 is transparent.
 *
 * The VDX7 written (the layout Load_DAV, Vdx7 and LoadTexturePages read):
 *   "VDX7", "PSXC" (PSXDAV_TAG), 12 bytes, +0x14 u32 directory offset
 *   directory {u16 entryCount, rectCount, pageCount; u32 entries, rects, pages, idLists}
 *   entries  u16 rect per texture id;  rects {u16 x, w, y, h, page};  idLists {u32 count; {u32 id | n << 16;
 *   u32 file offset of an entry[n]}};  pages {u16 x0, x1, y0, y1, format; w * h u16 texels}; "END "
 * Every material becomes one rectangle (decoded through its palette), packed into 256 x 256 pages: the images with
 * no transparent texel on immediate (opaque) ARGB1555 pages, first, as LoadTexturePages expects; the others on blend
 * pages in ARGB4444, as the PC keeps its cut-out images. There is always at least one blend page, as LoadTexturePages
 * also expects.
 */
#include "langexport_psxdav.h"
#include "langexport.h"

#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>


namespace {

const u32 PAGE = 256;          /* the PC page size */
const u16 FMT_OPAQUE_1555 = 5; /* TEXFMT_KIND_IMMEDIATE | ARGB1555 */
const u16 FMT_BLEND_4444 = 10; /* TEXFMT_KIND_BLEND | ARGB4444 */

struct Reader {
    const u8 *d;
    u32 size;
    bool ok = true;
    u16 U16(u32 at)
    {
        if (at + 2 > size) {
            ok = false;
            return 0;
        }
        return (u16)(d[at] | d[at + 1] << 8);
    }
    u32 U32(u32 at) { return U16(at) | (u32)U16(at + 2) << 16; }
};

struct Rect16 {
    u16 x, w, y, h, page, mode;
};

struct Page {
    u16 w, h; /* in 16-bit units and rows */
    u32 at;   /* file offset of the data */
};

/* a material's image, as PC ARGB1555 texels (bit 15 set = transparent) */
struct Image {
    u32 w = 0, h = 0;
    std::vector<u16> texels;
    bool transparent = false;   /* has a transparent texel: goes on a blend page */
    u32 x = 0, y = 0, page = 0; /* where it was packed */
};

struct PsxFile {
    Reader r;
    std::vector<u16> matTexture, matPalette;
    std::vector<Rect16> textures, cluts;
    std::vector<Page> pages;
    u32 matAt = 0, expAt = 0;
};

u16 ToPc1555(u16 c)
{
    if (c == 0)
        return 0x8000;
    return (u16)((c & 31) << 10 | (c >> 5 & 31) << 5 | (c >> 10 & 31));
}

/* ARGB1555 (bit 15 = transparent) to the PC's ARGB4444, whose top nibble is the transparency (0 = opaque) */
u16 To4444(u16 c)
{
    if (c & 0x8000)
        return 0xf000;
    return (u16)((c >> 11 & 15) << 8 | (c >> 6 & 15) << 4 | (c >> 1 & 15));
}

bool Parse(PsxFile &f)
{
    Reader &r = f.r;
    u16 nMat = r.U16(0x14), nTex = r.U16(0x16), nClut = r.U16(0x18), nPages = r.U16(0x1a);
    u32 matAt = r.U32(0x1c), texAt = r.U32(0x20), clutAt = r.U32(0x24), endAt = r.U32(0x28), expAt = r.U32(0x2c);
    f.matAt = matAt;
    f.expAt = expAt;
    for (u32 i = 0; i < nMat; i++) {
        f.matTexture.push_back(r.U16(matAt + 6 * i));
        f.matPalette.push_back(r.U16(matAt + 6 * i + 2));
    }
    for (u32 i = 0; i < nTex + nClut; i++) {
        u32 at = i < nTex ? texAt + 12 * i : clutAt + 12 * (i - nTex);
        Rect16 t = {r.U16(at), r.U16(at + 2), r.U16(at + 4), r.U16(at + 6), r.U16(at + 8), r.U16(at + 10)};
        (i < nTex ? f.textures : f.cluts).push_back(t);
    }
    u32 at = endAt + 4;
    for (u32 i = 0; i < nPages && r.ok; i++) {
        Page p = {r.U16(at + 2), r.U16(at + 6), at + 8};
        at = p.at + (u32)p.w * p.h * 2;
        if (at > r.size)
            r.ok = false;
        f.pages.push_back(p);
    }
    return r.ok && nMat && !f.pages.empty();
}

/* the u16 at (x, y) of a page, in 16-bit units. x may run past the row: a 256-colour palette on the 128-unit-wide
 * page 0 goes on into the next row, as it does in the file. */
u16 PageU16(PsxFile &f, u32 page, u32 x, u32 y)
{
    if (page >= f.pages.size())
        return 0;
    const Page &p = f.pages[page];
    return f.r.U16(p.at + (y * p.w + x) * 2);
}

u8 PageByte(PsxFile &f, u32 page, u32 xByte, u32 y)
{
    if (page >= f.pages.size())
        return 0;
    const Page &p = f.pages[page];
    u32 at = p.at + y * p.w * 2 + xByte;
    return at < f.r.size ? f.r.d[at] : 0;
}

bool Decode(PsxFile &f, u32 material, Image &out)
{
    u32 ti = f.matTexture[material], ci = f.matPalette[material];
    if (ti >= f.textures.size())
        return false;
    const Rect16 &t = f.textures[ti];
    u32 perUnit = t.mode == 2 ? 4 : t.mode == 1 ? 2 : 1;
    out.w = t.w * perUnit;
    out.h = t.h;
    if (!out.w || !out.h || out.w > PAGE || out.h > PAGE)
        return false;
    u16 palette[256] = {0};
    if (perUnit > 1) {
        if (ci >= f.cluts.size())
            return false;
        const Rect16 &c = f.cluts[ci];
        for (u32 k = 0; k < (perUnit == 4 ? 16u : 256u); k++)
            palette[k] = PageU16(f, c.page, c.x + k, c.y);
    }
    out.texels.resize(out.w * out.h);
    for (u32 y = 0; y < out.h; y++) {
        for (u32 x = 0; x < out.w; x++) {
            u16 c;
            if (perUnit == 4) {
                u8 b = PageByte(f, t.page, t.x * 2 + x / 2, t.y + y);
                c = palette[x & 1 ? b >> 4 : b & 15];
            } else if (perUnit == 2) {
                c = palette[PageByte(f, t.page, t.x * 2 + x, t.y + y)];
            } else {
                c = PageU16(f, t.page, t.x + x, t.y + y);
            }
            out.texels[y * out.w + x] = ToPc1555(c);
            out.transparent |= c == 0;
        }
    }
    return true;
}

/* shelf packing into 256 x 256 pages, tallest first; returns the page count */
u32 Pack(std::vector<Image *> images)
{
    std::stable_sort(images.begin(), images.end(), [](const Image *a, const Image *b) { return a->h > b->h; });
    u32 page = 0, x = 0, y = 0, shelf = 0;
    bool any = false;
    for (Image *im : images) {
        if (x + im->w > PAGE) {
            x = 0;
            y += shelf;
            shelf = 0;
        }
        if (y + im->h > PAGE) {
            page++;
            x = y = shelf = 0;
        }
        im->x = x;
        im->y = y;
        im->page = page;
        x += im->w;
        shelf = std::max(shelf, im->h);
        any = true;
    }
    return any ? page + 1 : 0;
}

struct Writer {
    std::vector<u8> b;
    void U16(u16 v)
    {
        b.push_back((u8)v);
        b.push_back((u8)(v >> 8));
    }
    void U32(u32 v)
    {
        U16((u16)v);
        U16((u16)(v >> 16));
    }
    void Put16(u32 at, u16 v)
    {
        b[at] = (u8)v;
        b[at + 1] = (u8)(v >> 8);
    }
    void Put32(u32 at, u32 v)
    {
        Put16(at, (u16)v);
        Put16(at + 2, (u16)(v >> 16));
    }
    void Align4()
    {
        while (b.size() & 3)
            b.push_back(0);
    }
};

std::vector<u8> Convert(PsxFile &f)
{
    u32 nMat = (u32)f.matTexture.size();
    /* one image per distinct (texture, palette) */
    std::vector<Image> images;
    std::vector<u32> rectOf(nMat, 0);
    std::vector<std::pair<u32, u32>> keys;
    for (u32 m = 0; m < nMat; m++) {
        std::pair<u32, u32> key(f.matTexture[m], f.matPalette[m]);
        auto it = std::find(keys.begin(), keys.end(), key);
        if (it != keys.end()) {
            rectOf[m] = (u32)(it - keys.begin());
            continue;
        }
        Image im;
        if (!Decode(f, m, im)) { /* a 1 x 1 transparent texel keeps the ids in step */
            im.w = im.h = 1;
            im.texels.assign(1, 0x8000);
            Export_Logf("PsxDav: material %u (texture %u) could not be decoded", m, f.matTexture[m]);
        }
        rectOf[m] = (u32)images.size();
        keys.push_back(key);
        images.push_back(std::move(im));
    }
    std::vector<Image *> opaque, cutOut;
    for (Image &im : images)
        (im.transparent ? cutOut : opaque).push_back(&im);
    u32 opaquePages = Pack(opaque);
    u32 blendPages = std::max(Pack(cutOut), 1u);
    for (Image *im : cutOut)
        im->page += opaquePages;

    /* the exported lists: id -> materials */
    std::vector<std::pair<u16, std::vector<u32>>> lists;
    {
        Reader &r = f.r;
        u32 count = r.U32(f.expAt), at = f.expAt + 4;
        for (u32 i = 0; i < count && r.ok; i++) {
            u16 id = r.U16(at), n = r.U16(at + 2);
            std::vector<u32> mats;
            for (u32 k = 0; k < n; k++) {
                u32 off = r.U32(at + 4 + 4 * k);
                if (off >= f.matAt && (off - f.matAt) % 6 == 0 && (off - f.matAt) / 6 < nMat)
                    mats.push_back((off - f.matAt) / 6);
            }
            at += 4 + 4 * n;
            lists.push_back({id, mats});
        }
    }

    Writer w;
    w.b.insert(w.b.end(), {'V', 'D', 'X', '7'});
    w.U32(PSXDAV_TAG);
    for (int i = 0; i < 3; i++)
        w.b.insert(w.b.end(), {'C', 'H', 'E', 'K'});
    w.U32(0x18); /* the directory */
    u32 dir = (u32)w.b.size();
    w.U16((u16)nMat);
    w.U16((u16)images.size());
    w.U16((u16)(opaquePages + blendPages));
    w.U32(0); /* entries */
    w.U32(0); /* rects */
    w.U32(0); /* pages (also the size Load_DAV reads) */
    w.U32(0); /* id lists */
    w.Align4();
    u32 entriesAt = (u32)w.b.size();
    for (u32 m = 0; m < nMat; m++)
        w.U16((u16)rectOf[m]);
    w.Align4();
    u32 rectsAt = (u32)w.b.size();
    for (Image &im : images) {
        w.U16((u16)im.x);
        w.U16((u16)im.w);
        w.U16((u16)im.y);
        w.U16((u16)im.h);
        w.U16((u16)im.page);
    }
    w.Align4();
    u32 listsAt = (u32)w.b.size();
    w.U32((u32)lists.size());
    for (auto &l : lists) {
        w.U32(l.first | (u32)l.second.size() << 16);
        for (u32 m : l.second)
            w.U32(entriesAt + 2 * m);
    }
    w.Align4();
    u32 pagesAt = (u32)w.b.size();
    w.Put32(dir + 6, entriesAt);
    w.Put32(dir + 10, rectsAt);
    w.Put32(dir + 14, pagesAt);
    w.Put32(dir + 18, listsAt);

    std::vector<u16> texels(PAGE * PAGE);
    for (u32 p = 0; p < opaquePages + blendPages; p++) {
        bool blend = p >= opaquePages;
        std::fill(texels.begin(), texels.end(), (u16)0x8000);
        for (Image &im : images)
            if (im.page == p)
                for (u32 y = 0; y < im.h; y++)
                    std::copy_n(&im.texels[y * im.w], im.w, &texels[(im.y + y) * PAGE + im.x]);
        w.U16(0);
        w.U16(PAGE);
        w.U16(0);
        w.U16(PAGE);
        w.U16(blend ? FMT_BLEND_4444 : FMT_OPAQUE_1555);
        for (u16 t : texels)
            w.U16(blend ? To4444(t) : t);
    }
    w.b.insert(w.b.end(), {'E', 'N', 'D', ' '});
    Export_Logf("PsxDav: %u materials as %u images, on %u opaque and %u cut-out pages", nMat,
                 (unsigned)images.size(), opaquePages, (unsigned)cutOut.size() ? blendPages : 0u);
    return std::move(w.b);
}

/* the last conversion: a level's readers ask for the same file one after another */
std::vector<u8> s_cacheIn, s_cacheOut;

} // namespace

int ExportPsxDav_Is(const void *data, u32 size)
{
    const u8 *d = (const u8 *)data;
    return d && size >= 0x44 && !memcmp(d + 4, "V2.6", 4) && !memcmp(d + 0x30, "----Section material", 20);
}

char *ExportPsxDav_ToVdx7(const void *data, u32 size, u32 *outSize)
{
    *outSize = 0;
    if (!ExportPsxDav_Is(data, size))
        return 0;
    const u8 *d = (const u8 *)data;
    if (s_cacheIn.size() != size || memcmp(s_cacheIn.data(), d, size) != 0) {
        PsxFile f;
        f.r.d = d;
        f.r.size = size;
        if (!Parse(f)) {
            Export_Logf("PsxDav: not a PlayStation .DAV this reader understands");
            return 0;
        }
        s_cacheOut = Convert(f);
        s_cacheIn.assign(d, d + size);
    }
    char *out = new char[s_cacheOut.size()];
    memcpy(out, s_cacheOut.data(), s_cacheOut.size());
    *outSize = (u32)s_cacheOut.size();
    return out;
}
