/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=1:7:7,1:8:8 IDO52_NO_LOOP_TEST_REPLACEMENT=occ:1 IDO52_EXCLUDE_REGS_WEB=44:s8 */
/* @CompileFlags: -Wc,-pic0 */
/* Address: 0x80144044   Size: 924 bytes (231 instructions) */
/* @PostProc: remap_stack_slots --map '72:84,76:84,120:100,124:104,132:112,136:116' */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xd4 */
/* @PostProc: swap_instructions --offset1 0x148 --offset2 0x150 */
/* @PostProc: patch_insn_reg_field 0x10c:rs:t4:fp 0x10c:rt:fp:t4 0x114:rt:s1:s2 0x118:rd:s2:s1 0x118:rt:s1:s2 0x12c:rs:t7:s0 0x12c:rt:s0:t7 0x134:rs:s2:s1 0x144:rs:s1:s2 0x144:rt:s1:s2 0x14c:rs:s1:s2 0x150:rs:s2:s1 0x150:rt:s2:s1 */
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

extern Mtx GsWSMATRIX;
extern s32 activeBuff;
extern s16 taco_grid_ofs;
extern s16 taco_grid_ofs_1;
extern s16 taco_grid_ofs_2;
extern s16 taco_grid_ofs_3;
extern s32 taco_grid_u_tbl;
extern s32 taco_grid_v_tbl;
extern s16 screen_w[];
extern u8  ot_buff0[];

extern Spr *prim_alloc(s32, s32);
extern void SetPolyFT4(Spr *);
extern s16  GetTPage(s32, s32, s32, s32);
extern s16  GetClut(s32, s32);
extern void AddPrim(u8 *, Spr *);
extern void SetTransMatrix(Mtx *);
extern void SetRotMatrix(Mtx *);
extern void RotTransPers(Svec *, Dvec *, s32 *, s32 *);

void
taco_grid_disp(void)
{
    s32   unused0[2];
    Mtx   a;
    Svec  v;
    Dvec  arr[13][9];
    s32   p;
    s32   flg;
    s32   unused2;
    s32   c0;
    s32   c1;
    s32   i;
    s32   k;
    s32   kk;
    s32   x;
    s32   zz;
    s32   h;
    s32   n;
    volatile u8 *pa;
    volatile u8 *pb;
    Spr  *s;

    c0 = taco_grid_u_tbl;
    c1 = taco_grid_v_tbl;
    a = GsWSMATRIX;
    SetTransMatrix(&a);
    SetRotMatrix(&a);

    v.vy = taco_grid_ofs_1;
    zz = 0;
    for (k = 0; k < 9; k++) {
        if (k == 8) {
            v.vz = (s16)zz;
        } else {
            v.vz = (s16)(zz + taco_grid_ofs_2);
        }
        for (i = 0, x = 12288; i < 13; i++) {
            v.vx = (s16)(x + taco_grid_ofs);
            RotTransPers(&v, &arr[i][k], &p, &flg);
            x -= 2048;
        }
        zz -= 4096;
    }

    for (kk = 0; kk < 8; kk++) {
        for (i = 0; i < 12; i++) {
            if (arr[i + 1][kk + 1].vx < 0) {
                continue;
            }
            if (arr[i][kk + 1].vx >= 513) {
                continue;
            }
            h = screen_w[activeBuff + 12];
            if (arr[i][kk].vy < h - 512) {
                continue;
            }
            if (h + 1024 < arr[i][kk + 1].vy) {
                continue;
            }
            s = prim_alloc(40, activeBuff);
            SetPolyFT4(s);
            s->r0 = 128;
            s->g0 = 128;
            s->b0 = 128;
            s->tpage = GetTPage(1, 0, 512, 256);
            s->clut = GetClut(0, 493);
            n = (i + kk + taco_grid_ofs_3) & 7;
            if (n >= 4) {
                n = 0;
            }
            pa = (volatile u8 *)&c0 + n;
            pb = (volatile u8 *)&c1 + n;
            s->u0 = *pa;
            s->v0 = *pb;
            s->u1 = (u8)(*pa + 63);
            s->v1 = *pb;
            s->u2 = *pa;
            s->v2 = (u8)(*pb + 63);
            s->u3 = (u8)(*pa + 63);
            s->v3 = (u8)(*pb + 63);
            s->x0 = arr[i][kk].vx;
            s->y0 = arr[i][kk].vy;
            s->x1 = arr[i][kk + 1].vx;
            s->y1 = arr[i][kk + 1].vy;
            s->x2 = arr[i + 1][kk].vx;
            s->y2 = arr[i + 1][kk].vy;
            s->x3 = arr[i + 1][kk + 1].vx;
            s->y3 = arr[i + 1][kk + 1].vy;
            AddPrim(&ot_buff0[(activeBuff << 10) + 1016], s);
        }
    }
}
