/* @PostProc: patch_insn_reg_field 0xa4:rt:t1:t0 0xa8:rs:t1:t0 0xa8:rt:t1:t0 0xac:rt:t3:t2 0xb0:rd:t2:t1 0xb0:rt:t1:t0 0xb4:rd:t3:t2 0xb4:rs:t3:t2 0xb4:rt:t2:t1 0xb8:rs:t3:t2 0xb8:rt:t3:t2 0xc4:rs:t9:t2 0xc4:rt:t3:t9 0x14c:rt:t1:t0 0x154:rs:t1:t0 0x158:rt:t2:t1 0x160:rs:t2:t1 0x164:rt:t0:t2 0x16c:rs:t0:t2 0x1d0:rt:t1:t0 0x1d8:rs:t1:t0 0x1dc:rt:t2:t1 0x1e4:rs:t2:t1 0x1e8:rt:t0:t2 0x1f0:rs:t0:t2 0x2a0:rt:t1:t0 0x2a4:rs:t1:t0 0x2a4:rt:t1:t0 0x2a8:rt:t0:t2 0x2ac:rs:t0:t2 0x2ac:rt:t0:t2 0x2b0:rd:t2:t1 0x2b0:rt:t1:t0 0x2b4:rs:t2:t1 0x2b4:rt:t0:t2 */
/* Address: 0x80144988 */
#include "types.h"

typedef struct {
    u32 tag;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
    s16 x3;
    s16 y3;
} TacoPolyF4;

extern s32 activeBuff;
extern s16 disp_ofs_y[];
extern u8 ot_buff0[];

extern void *prim_alloc();
extern void safe_env();
extern s32 SetDrawMode();
extern s32 GetTPage();
extern void AddPrim();
extern void SetSemiTrans();
extern void SetShadeTex();
extern void SetPolyF4();
extern s32 rand();

void
taco_star_disp(s32 *pos, s32 dx, s32 dy)
{
    TacoPolyF4 *prim[101];
    void *tpg;
    s32 i;
    s32 x;
    s32 y;
    s32 sy;

    for (i = 0; i < 25; i++) {
        prim[i] = (TacoPolyF4 *)prim_alloc(24, activeBuff);
        SetPolyF4(prim[i]);
        SetSemiTrans(prim[i], 1);
        SetShadeTex(prim[i], 0);

        pos[i * 2] = pos[i * 2] + dx;
        pos[i * 2 + 1] = pos[i * 2 + 1] + dy;
        x = pos[i * 2];
        sy = disp_ofs_y[activeBuff] + pos[i * 2 + 1];

        if (x < 0 || x >= 321) {
            if (dx > 0) {
                pos[i * 2] = 0;
            } else {
                pos[i * 2] = 320;
            }
            pos[i * 2 + 1] = rand() % 240 - 120;
        } else if (sy < 0 || sy >= 481) {
            pos[i * 2] = rand() % 320;
            if (dy > 0) {
                pos[i * 2 + 1] = -120;
            } else {
                pos[i * 2 + 1] = 120;
            }
        }

        prim[i]->x0 = x;
        prim[i]->y0 = sy;
        prim[i]->x1 = x + 1;
        prim[i]->y1 = sy;
        prim[i]->x2 = x;
        prim[i]->y2 = sy + 1;
        prim[i]->x3 = x + 1;
        prim[i]->y3 = sy + 1;

        if (rand() % 5 >= 3) {
            prim[i]->r0 = 128;
            prim[i]->g0 = 128;
            prim[i]->b0 = 128;
        } else {
            prim[i]->r0 = 255;
            prim[i]->g0 = 255;
            prim[i]->b0 = 255;
        }

        if (rand() % 3 > 0) {
            AddPrim(&ot_buff0[(activeBuff << 10) + 400], prim[i]);
        }
    }

    tpg = prim_alloc(12, activeBuff);
    SetDrawMode(tpg, 0, 0, GetTPage(1, 1, 0, 0), 0);
    AddPrim(&ot_buff0[activeBuff << 10], tpg);
    safe_env(0);
}
