/* Address: 0x80049A58 */
/* Size: 1452 bytes (363 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:a2,172:a1,106:t2 */
/* @CompileEnv: IDO52_FORCE_WEB_RANGE_SPLIT=13 IDO52_SPLIT_BLOCKS=13.3#1:11 IDO52_FORCE_WEB_NOSPILL=13.0:5,13.1/-1:5,13.2/-1:5,13.3/-1:5 */
/* @CompileEnv: IDO52_UGEN_DEST_TEMP=12,13,15,16,18,19,21,22,23,24,26,27 */
/* @PostProc: strip_leading_instructions --count auto */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=11:t6,12:t7,13:t3,14:t3,15:t4,16:t8,18:t3,19:t4 */
#include "types.h"

typedef struct {
    u32 tag;
    u8  r0, g0, b0, code;
    s16 x0, y0;
    u8  r1, g1, b1, pad1;
    s16 x1, y1;
    u8  r2, g2, b2, pad2;
    s16 x2, y2;
    u8  r3, g3, b3, pad3;
    s16 x3, y3;
} POLY_G4;

extern s32 activeBuff;
extern u8  screen_w[];
extern u8  ot_buff0[];

#define ENV_X(i) (*(s16 *)(screen_w + (i) * 2 + 20))
#define ENV_Y(i) (*(s16 *)(screen_w + (i) * 2 + 24))

#define setRGB0(p, _r, _g, _b) ((p)->r0 = (_r), (p)->g0 = (_g), (p)->b0 = (_b))
#define setRGB1(p, _r, _g, _b) ((p)->r1 = (_r), (p)->g1 = (_g), (p)->b1 = (_b))
#define setRGB2(p, _r, _g, _b) ((p)->r2 = (_r), (p)->g2 = (_g), (p)->b2 = (_b))
#define setRGB3(p, _r, _g, _b) ((p)->r3 = (_r), (p)->g3 = (_g), (p)->b3 = (_b))

extern void *prim_alloc(s32 size, s32 bufIdx);
extern void  SetPolyG4(POLY_G4 *prim);
extern void  SetSemiTrans(POLY_G4 *prim, s32 semiTrans);
extern void  AddPrim(void *otEntry, POLY_G4 *prim);

void _sprite_set_box_shade(s32 xoff, s32 yoff, s32 xoff2, s32 yoff2,
                   s32 slot, u32 rgb, s32 flags)
{
    s32 r;
    POLY_G4 *p;
    s32 g;
    s32 b;

    p = (POLY_G4 *)prim_alloc(36, activeBuff);
    SetPolyG4(p);
    if (flags & 0x80) {
        SetSemiTrans(p, 1);
    } else {
        SetSemiTrans(p, 0);
    }

    switch (flags & 0x7F) {
    case 6:
        r = rgb >> 16;
        g = rgb >> 8;
        setRGB0(p, 0, 0, 0);
        setRGB1(p, r, g, rgb);
        setRGB2(p, 0, 0, 0);
        setRGB3(p, r, g, rgb);
        break;
    case 5:
        r = rgb >> 16;
        g = rgb >> 8;
        setRGB0(p, r, g, rgb);
        setRGB1(p, 0, 0, 0);
        setRGB2(p, r, g, rgb);
        setRGB3(p, 0, 0, 0);
        break;
    case 4:
        r = rgb >> 16;
        g = rgb >> 8;
        setRGB0(p, 0, 0, 0);
        setRGB1(p, 0, 0, 0);
        setRGB2(p, r, g, rgb);
        setRGB3(p, r, g, rgb);
        break;
    case 3:
        g = (rgb >> 8) << 3;
        r = (rgb >> 16) << 3;
        p->g3 = (u8)g; p->r3 = (u8)r;
        p->b3 = (u8)g;
        p->r2 = (u8)r;
        p->g2 = (u8)g;
        p->b2 = (u8)g;
        p->r1 = (u8)r;
        p->g1 = (u8)g;
        p->b1 = (u8)g;
        p->r0 = (u8)r;
        p->g0 = (u8)g;
        p->b0 = (u8)g;
        break;
    case 1:
        r = rgb >> 16;
        g = rgb >> 8;
        setRGB3(p, r, g, rgb);
        setRGB2(p, r, g, rgb);
        setRGB1(p, r, g, rgb);
        setRGB0(p, r, g, rgb);
        break;
    case 2:
        r = (s32)((rgb >> 16) & 0xFF);
        g = (s32)((rgb >> 8) & 0xFF);
        b = (s32)(rgb & 0xFF);
        p->r3 = (u8)((r << 3) / 10);
        p->g3 = (u8)((g << 3) / 10);
        p->b3 = (u8)((b << 3) / 10);
        p->r1 = (u8)((r * 10) / 10);
        p->r2 = (u8)((r * 10) / 10);
        p->r0 = (u8)((r * 10) / 10);
        p->g1 = (u8)((g * 10) / 10);
        p->g2 = (u8)((g * 10) / 10);
        p->g0 = (u8)((g * 10) / 10);
        p->b1 = (u8)((b * 10) / 10);
        p->b2 = (u8)((b * 10) / 10);
        p->b0 = (u8)((b * 10) / 10);
        break;
    case 0:
    default:
        r = (s32)((rgb >> 16) & 0xFF);
        g = (s32)((rgb >> 8) & 0xFF);
        b = (s32)(rgb & 0xFF);
        setRGB3(p, r << 2, g << 2, b << 2);
        setRGB2(p, r * 6, g * 6, b * 6);
        setRGB1(p, r << 3, g << 3, b << 3);
        setRGB0(p, r * 10, g * 10, b * 10);
        break;
    }

    p->x0 = (s16)(ENV_X(activeBuff) + xoff);
    p->y0 = (s16)(ENV_Y(activeBuff) + yoff);
    p->x1 = (s16)(ENV_X(activeBuff) + xoff + xoff2);
    p->y1 = (s16)(ENV_Y(activeBuff) + yoff);
    p->x2 = (s16)(ENV_X(activeBuff) + xoff);
    goto L;
L:
    p->y2 = (s16)(ENV_Y(activeBuff) + yoff + yoff2);
    p->x3 = (s16)(ENV_X(activeBuff) + xoff + xoff2);
    p->y3 = (s16)(ENV_Y(activeBuff) + yoff + yoff2);

    AddPrim(&ot_buff0[(activeBuff * 1024) + (slot << 2)], p);
}
