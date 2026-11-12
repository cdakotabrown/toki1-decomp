/* Address: 0x8004E228 */
/* Size: 780 bytes (195 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileEnv: IDO52_COPYPROP_VAR=-76 IDO52_FORCE_WEB_COLOR=71:s2,8:s8 IDO52_UGEN_TEMP_REG=25:t1,26:t2,35:t0 IDO52_AS1_TIE_REJECT=359,363 */
/* @CompileFlags: -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s32 *pal;
    u32 *map;
    u8 mode;
    u8 pad[3];
    u8 cols;
    u8 rows;
    u8 tileW;
    u8 tileH;
    s32 f10;
} TileInfo;

typedef struct {
    u32 flags;
    s16 x;
    s16 y;
    s16 w;
    s16 h;
    u16 tpage;
    u8 u;
    u8 v;
    s16 x2;
    s16 y2;
    u8 r;
    u8 g;
    u8 b;
    u8 pad2;
    s16 tw2;
    s16 th2;
    s16 sx;
    s16 sy;
    u32 flags2;
} Sprite;

typedef struct {
    u8 b[20];
} Clut;

extern TileInfo mod_w[];
extern s32 activeBuff;
extern Clut ot_work[];

extern s32 get_brightness();
extern s32 search_tpage_multi();
extern void GsSortFastSprite(Sprite *, Clut *, u16);

void mod_set(n, base, x0, y0, sx, sy, mode)
s32 n;
s32 base;
s32 x0;
s32 y0;
s32 sx;
s32 sy;
u16 mode;
{
    TileInfo *info;
    s32 col;
    u32 *src;
    u32 b2;
    s32 row;
    Sprite spr;
    u32 b1;
    s32 x;
    s32 y;
    s32 bright;
    u8 tileW;
    u16 m;
    s32 padB[10];
    s32 cs;
    u32 b3;
    s32 padC[5];

    bright = get_brightness();
    switch (mod_w[n].mode) {
    case 0:
        spr.flags = 0x1000000;
        cs = 1;
        break;
    case 1:
        cs = 2;
        spr.flags = 0;
        break;
    case 2:
        cs = 2;
        spr.flags = 0;
        break;
    case 3:
        spr.flags = 0x1000000; cs = 1;
        break;
    case 4:
        spr.flags = 0x1000000; cs = 1;
        break;
    case 5:
        spr.flags = 0x1000000; cs = 1;
        break;
    default:
        spr.flags = 0x1000000;
        cs = 1;
        break;
    }
    tileW = mod_w[n].tileW;
    spr.w = tileW;
    spr.h = mod_w[n].tileH;
    spr.tw2 = 0;
    spr.th2 = 0;
    spr.r = bright;
    spr.g = bright;
    spr.b = bright;
    spr.flags2 = 0;
    spr.sx = 4096;
    spr.sy = 4096;
    src = mod_w[n].map + mod_w[n].cols * (mod_w[n].rows * base);
    for (row = 0; row < mod_w[n].rows; row++) {
        for (col = 0; col < mod_w[n].cols; col++) {
            spr.x = col * tileW + x0;
            spr.y = row * tileW + y0;
            m = mode;
            b2 = (*src & 0xFF0000) >> 16;
            b3 = *src & 0xFF000000;
            b1 = (*src & 0xFF00) >> 8;
            spr.tpage = search_tpage_multi(mod_w[n].pal[b3 >> 24], cs);
            switch (tileW) {
            default:
                spr.u = (b2 & 0xF) << 3;
                spr.v = ((b2 & 0xF0) >> 4) << 3;
                break;
            case 8:
                spr.u = (b2 & 0xF) << 3;
                spr.v = ((b2 & 0xF0) >> 4) << 3;
                break;
            case 16:
                spr.u = (b2 & 0x7) << 4;
                spr.v = ((b2 & 0xF8) >> 4) << 4;
                break;
            }
            spr.x2 = sx + (b1 << 4);
            spr.y2 = sy;
            GsSortFastSprite(&spr, &ot_work[activeBuff], m);
            src++;
        }
    }
}
