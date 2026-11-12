/* Address: 0x8013F378 */
/* Size: 880 bytes (220 instructions) */
/* @PostProc: patch_stack_complete --old 160 --new 200 */
/* @PostProc: swap_instructions --offset1 184 --offset2 188 */
/* @PostProc: swap_instructions --offset1 220 --offset2 232 */
/* @PostProc: swap_instructions --offset1 284 --offset2 292 */
/* @PostProc: swap_instructions --offset1 404 --offset2 408 */
/* @PostProc: swap_instructions --offset1 412 --offset2 416 */
/* @PostProc: swap_instructions --offset1 684 --offset2 688 */
/* @PostProc: remap_stack_slots --map '96:104,100:108,104:136,106:138,108:140,110:142,112:144,114:146,116:148,118:150,120:152,122:154,124:156,128:160' */
/* @PostProc: patch_insn_reg_field 0x194:rt:a2:v1 0x198:rt:v1:a2 0x19c:rt:s4:s2 0x1b0:rt:s4:s2 0x1b4:rt:s4:s2 0x1b8:rt:v1:a2 0x1bc:rt:v1:a2 0x1c0:rt:a2:v1 0x1c4:rt:a2:v1 0x1d8:rt:s2:s4 0x1e4:rs:s2:s4 0x1e4:rt:s2:s4 0x234:rs:s2:s4 0x2ac:rt:a2:v1 0x2b0:rt:v1:a2 0x2b8:rt:v1:a2 0x2bc:rt:v1:a2 0x2c0:rt:a2:v1 0x2c4:rt:a2:v1 0x2c8:rt:s4:s2 0x2d0:rt:s4:s2 0x344:rs:s2:s4 */
#include "types.h"

typedef struct { s32 w[8]; } Mtx;
typedef struct { s16 vx, vy; } Dvec;
typedef struct { s16 vx, vy, vz, pad; } Svec;

typedef struct Spr {
    u32 tag;
    u8  r0, g0, b0, code;
    s16 x0, y0;
    u8  u0, v0;
    s16 clut;
    s16 x1, y1;
    u8  u1, v1;
    s16 tpage;
    s16 x2, y2;
    u8  u2, v2;
    s16 pad30;
    s16 x3, y3;
    u8  u3, v3;
    s16 pad38;
} Spr;

typedef struct Cam {
    u8  unk00[116];
    s16 f116;
    s16 f118;
} Cam;

extern Mtx GsWSMATRIX;
extern s32 activeBuff;
extern u8  ot_buff0[];
extern u32 frame_counter;
extern Cam *taco_actor_rec;

extern Spr *prim_alloc(s32, s32);
extern void SetPolyFT4(Spr *);
extern s32  SetSemiTrans(Spr *, s32);
extern s16  GetTPage(s32, s32, s32, s32);
extern s16  GetClut(s32, s32);
extern void AddPrim(u8 *, Spr *);
extern void SetTransMatrix(Mtx *);
extern void SetRotMatrix(Mtx *);
extern void RotTransPers(Svec *, Dvec *, s32 *, s32 *);

void
taco_board_disp(void)
{
    Mtx  a;
    Svec v;
    Dvec out[2][2];
    s32  sz;
    s32  flg;
    s32  yofs;
    s32  xofs;
    s32  i;
    s32  j;
    Spr  *s;

    a = GsWSMATRIX;
    SetTransMatrix(&a);
    SetRotMatrix(&a);

    v.vz = -28672;
    yofs = 0;
    for (j = 0; j != 2; j++) {
        v.vy = (s16)(taco_actor_rec->f118 + yofs);
        xofs = 0;
        for (i = 0; i < 2; i++) {
            v.vx = (s16)(taco_actor_rec->f116 + xofs - 256);
            RotTransPers(&v, &out[i][j], &sz, &flg);
            xofs += 512;
        }
        yofs += 512;
    }

    s = prim_alloc(40, activeBuff);
    SetPolyFT4(s);
    SetSemiTrans(s, 1);
    s->r0 = 128;
    s->g0 = 128;
    s->b0 = 128;
    s->tpage = GetTPage(0, 1, 832, 256);
    s->clut = GetClut(16, 495);
    s->u1 = 159;
    s->u3 = 159;
    s->u0 = 129;
    s->u2 = 129;
    s->v2 = 31;
    s->v3 = 31;
    s->v0 = 1;
    s->v1 = 1;
    s->x0 = out[0][0].vx;
    s->y0 = out[0][0].vy;
    s->x1 = out[0][1].vx;
    s->y1 = out[0][1].vy;
    s->x2 = out[1][0].vx;
    s->y2 = out[1][0].vy;
    s->x3 = out[1][1].vx;
    s->y3 = out[1][1].vy;
    AddPrim((u8 *)((s32)ot_buff0 + (activeBuff << 10) + 4), s);

    if ((frame_counter & 8) != 0) {
        s = prim_alloc(40, activeBuff);
        SetPolyFT4(s);
        SetSemiTrans(s, 1);
        s->r0 = 128;
        s->g0 = 128;
        s->b0 = 128;
        s->tpage = GetTPage(0, 1, 832, 256);
        s->clut = GetClut(16, 495);
        s->v2 = 63;
        s->v3 = 63;
        s->v0 = 33;
        s->v1 = 33;
        s->u0 = 129;
        s->u1 = 159;
        s->u2 = 129;
        s->u3 = 159;
        s->x0 = out[0][0].vx;
        s->y0 = out[0][0].vy;
        s->x1 = out[0][1].vx;
        s->y1 = out[0][1].vy;
        s->x2 = out[1][0].vx;
        s->y2 = out[1][0].vy;
        s->x3 = out[1][1].vx;
        s->y3 = out[1][1].vy;
        AddPrim((u8 *)((s32)ot_buff0 + (activeBuff << 10) + 4), s);
    }
}
