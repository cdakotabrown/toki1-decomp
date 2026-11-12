/* Address: 0x80142FB0 */
/* Size: 544 bytes (136 insns) */
#include "types.h"

typedef struct {
    u8  pad0[4];
    u8  f4;
    u8  f5;
    u8  f6;
    u8  pad1;
    s16 f8;
    s16 f10;
    u8  f12;
    u8  f13;
    s16 f14;
    s16 f16;
    s16 f18;
} Spr;

extern s16 screen_w[];
extern s32 frame_counter;
extern u8 ot_buff0[];
extern s32 activeBuff;
extern u8 taco_glyph_uv_tbl[];

extern s32 prim_alloc(s32, s32);
extern void dtd_on_tpage(s32, s32, s32, s32, s32);
extern s32 GetClut(s32, s32);
extern void AddPrim(s32, s32);
extern void SetSemiTrans(s32, s32);
extern void SetSprt(s32);

void taco_num_disp(s32 n, s32 x, s32 y, s32 kind)
{
    Spr *p;
    s32 v;
    s32 sgn;
    s32 ofs;
    s32 ten;

    if (n < 0) {
        v = -n;
        sgn = -1;
    } else {
        v = n;
        sgn = 1;
    }

    if (sgn == -1) {
        if (frame_counter & 4) {
            return;
        }
    }

    ofs = 0;
    ten = 10;
    while (v >= 0) {
        p = (Spr *)prim_alloc(20, activeBuff);
        SetSprt((s32)p);
        p->f16 = 12;
        p->f18 = 16;
        SetSemiTrans((s32)p, 0);
        p->f4 = 128;
        p->f5 = 128;
        p->f6 = 128;
        p->f8 = (s16)(screen_w[activeBuff + 10] + x - ofs);
        p->f10 = (s16)(screen_w[activeBuff + 12] + y);
        p->f12 = (u8)(taco_glyph_uv_tbl[(v % ten) * 2] + 128);
        p->f13 = taco_glyph_uv_tbl[(v % ten) * 2 + 1];
        p->f14 = (s16)GetClut(kind << 4, 495);
        AddPrim((s32)(ot_buff0 + (activeBuff << 10) + 4), (s32)p);
        if (v < 10) {
            break;
        }
        v = v / ten;
        ofs += 12;
    }

    dtd_on_tpage(832, 384, 1, 0, 0);
}
