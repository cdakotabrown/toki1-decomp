/* @PostProc: patch_insn_reg_field 0x140:rd:t5:t6 0x144:rs:t5:t6 0x144:rt:t6:t7 0x148:rt:t6:t7 0x14c:rt:t7:t8 0x150:rs:t7:t8 0x150:rt:t7:t8 0x158:rd:t8:t0 0x158:rs:t7:t8 0x15c:rs:t8:t0 0x15c:rt:t9:t1 0x164:rt:t9:t1 0x178:rt:t0:t2 0x17c:rs:t0:t2 0x17c:rt:t0:t2 0x184:rd:t1:t3 0x184:rt:t0:t2 0x188:rd:t2:t4 0x188:rs:t1:t3 0x18c:rt:t2:t4 0x194:rt:t3:t5 0x198:rs:t3:t5 0x198:rt:t3:t5 0x1a0:rd:t4:t6 0x1a0:rt:t3:t5 0x1a4:rd:t5:t7 0x1a4:rs:t4:t6 0x1a8:rt:t5:t7 0x1ac:rt:t6:t8 0x1b0:rs:t6:t8 0x1b0:rt:t6:t8 0x1b8:rd:t7:t9 0x1b8:rt:t6:t8 0x1bc:rd:t8:t1 0x1bc:rs:t7:t9 0x1c0:rs:t8:t1 0x1c0:rt:t9:t2 0x1c4:rt:t9:t2 0x1c8:rt:t0:t3 0x1cc:rs:t0:t3 0x1cc:rt:t0:t3 0x1d4:rd:t1:t4 0x1d4:rt:t0:t3 0x1d8:rd:t2:t6 0x1d8:rs:t1:t4 0x1dc:rs:t2:t6 0x1dc:rt:t3:t7 0x1e4:rt:t3:t7 0x1f8:rt:t4:t8 0x1fc:rs:t4:t8 0x1fc:rt:t4:t8 0x204:rd:t5:t9 0x204:rt:t4:t8 0x208:rd:t6:t0 0x208:rs:t5:t9 0x20c:rt:t6:t0 0x214:rt:t7:t1 0x218:rs:t7:t1 0x218:rt:t7:t1 0x220:rd:t8:t2 0x220:rt:t7:t1 0x224:rd:t9:t3 0x224:rs:t8:t2 0x228:rt:t9:t3 0x22c:rt:t0:t4 0x230:rs:t0:t4 0x230:rt:t0:t4 0x238:rd:t1:t5 0x238:rt:t0:t4 0x23c:rd:t2:t7 0x23c:rs:t1:t5 0x240:rs:t2:t7 0x240:rt:t3:t8 0x244:rt:t3:t8 0x248:rt:t4:t9 0x24c:rs:t4:t9 0x24c:rt:t4:t9 0x254:rd:t5:t0 0x254:rt:t4:t9 0x258:rd:t6:t2 0x258:rs:t5:t0 0x25c:rs:t6:t2 0x25c:rt:t7:t3 0x264:rt:t7:t3 0x278:rt:t8:t4 0x27c:rs:t8:t4 0x27c:rt:t8:t4 0x284:rd:t9:t5 0x284:rt:t8:t4 0x288:rd:t0:t6 0x288:rs:t9:t5 0x28c:rt:t0:t6 0x294:rt:t1:t7 0x298:rs:t1:t7 0x298:rt:t1:t7 0x2a0:rd:t2:t8 0x2a0:rt:t1:t7 0x2a4:rd:t3:t9 0x2a4:rs:t2:t8 0x2a8:rt:t3:t9 0x2ac:rt:t4:t0 0x2b0:rs:t4:t0 0x2b0:rt:t4:t0 0x2b8:rd:t5:t1 0x2b8:rt:t4:t0 0x2bc:rd:t6:t3 0x2bc:rs:t5:t1 0x2c0:rs:t6:t3 0x2c0:rt:t7:t4 0x2c4:rt:t7:t4 0x2c8:rt:t8:t5 0x2cc:rs:t8:t5 0x2cc:rt:t8:t5 0x2d4:rd:t9:t6 0x2d4:rt:t8:t5 0x2d8:rd:t0:t8 0x2d8:rs:t9:t6 0x2dc:rs:t0:t8 0x2dc:rt:t1:t9 0x2e0:rt:t1:t9 0x2e4:rt:t2:t0 0x2ec:rd:t3:t1 0x2ec:rs:t2:t0 0x2f0:rt:t3:t1 0x2f4:rt:t4:t2 0x2fc:rd:t5:t3 0x2fc:rs:t4:t2 0x300:rt:t5:t3 0x304:rt:t6:t4 0x30c:rd:t7:t5 0x30c:rs:t6:t4 0x310:rt:t7:t5 0x314:rt:t8:t6 0x31c:rd:t9:t7 0x31c:rs:t8:t6 0x320:rt:t9:t7 0x324:rt:t0:t8 0x32c:rs:t0:t8 0x32c:rt:t1:t9 0x334:rd:t2:t0 0x334:rs:t1:t9 0x338:rt:t2:t0 0x33c:rt:t3:t1 0x344:rd:t4:t2 0x344:rs:t3:t1 0x348:rt:t4:t2 0x34c:rt:t5:t3 0x354:rs:t5:t3 0x354:rt:t6:t4 0x35c:rd:t7:t5 0x35c:rs:t6:t4 0x360:rt:t7:t5 0x364:rt:t8:t6 0x36c:rd:t9:t7 0x36c:rs:t8:t6 0x374:rt:t9:t7 0x38c:rt:t0:t8 0x390:rs:t0:t8 0x390:rt:t0:t8 0x394:rt:t3:t1 0x398:rd:t1:t9 0x398:rt:t0:t8 0x39c:rs:t1:t9 0x39c:rt:t2:t0 0x3a0:rs:t3:t1 0x3a0:rt:t3:t1 0x3a4:rs:t2:t0 0x3a4:rt:t3:t1 0x3b0:rt:t4:t2 0x3b4:rt:t6:t4 0x3c0:rs:t4:t2 0x3c0:rt:t5:t3 0x3c4:rs:t6:t4 0x3c4:rt:t7:t5 0x3c8:rt:t7:t5 0x3cc:rt:t5:t3 */
/* Address: 0x8013DB8C */
/* Size: 1028 bytes (257 insns) */
#include "types.h"

typedef struct {
    u32 tag;
    u8  r;
    u8  g;
    u8  b;
    u8  code;
    s16 x0;
    s16 y0;
    u8  u0;
    u8  v0;
    u16 clut;
    s16 x1;
    s16 y1;
    u8  u1;
    u8  v1;
    u16 tpage;
    s16 x2;
    s16 y2;
    u8  u2;
    u8  v2;
    u16 pad2;
    s16 x3;
    s16 y3;
    u8  u3;
    u8  v3;
    u16 pad3;
} PolyFT4;

extern s32  activeBuff;
extern s16  screen_w[];
extern u32  ot_buff0[][256];
extern s16  taiiku_tama_gscrl_hi;
extern s16  taiiku_tama_gnd_y0[];
extern s16  taiiku_tama_gnd_y1[];
extern s16  taiiku_tama_gnd_x0[];
extern s16  taiiku_tama_gnd_x1[];
extern s16  taiiku_tama_gnd_x2[];
extern s16  taiiku_tama_gnd_x3[];

extern PolyFT4 *prim_alloc(s32, s32);
extern void SetPolyFT4(PolyFT4 *);
extern void SetSemiTrans(PolyFT4 *, s32);
extern void SetShadeTex(PolyFT4 *, s32);
extern u16  GetTPage(s32, s32, s32, s32);
extern u16  GetClut(s32, s32);
extern void AddPrim(u32 *, PolyFT4 *);

void taiiku_ground_disp(void)
{
    PolyFT4 *p;
    s32 i;
    s32 k;
    s16 sx;
    s16 sy;

    sx = screen_w[activeBuff + 10];
    sy = screen_w[activeBuff + 12];

    for (i = 0; i < 80; i++) {
        p = prim_alloc(40, activeBuff);
        SetPolyFT4(p);
        SetSemiTrans(p, 0);
        SetShadeTex(p, 0);

        k = (i >> 2) * 8;
        switch (i >> 2) {
        case 0:
            p->r = 104;
            p->g = 104;
            p->b = 104;
            p->u0 = 0;
            p->v0 = (taiiku_tama_gscrl_hi >> 3) + k;
            p->u1 = 64;
            p->v1 = (taiiku_tama_gscrl_hi >> 3) + k;
            p->u2 = 0;
            p->v2 = (taiiku_tama_gscrl_hi >> 3) + k + 8;
            p->u3 = 64;
            p->v3 = (taiiku_tama_gscrl_hi >> 3) + k + 8;
            break;
        case 1:
            p->r = 112;
            p->g = 112;
            p->b = 112;
            p->u0 = 0;
            p->v0 = (taiiku_tama_gscrl_hi >> 2) + k;
            p->u1 = 64;
            p->v1 = (taiiku_tama_gscrl_hi >> 2) + k;
            p->u2 = 0;
            p->v2 = (taiiku_tama_gscrl_hi >> 2) + k + 8;
            p->u3 = 64;
            p->v3 = (taiiku_tama_gscrl_hi >> 2) + k + 8;
            break;
        case 2:
            p->r = 120;
            p->g = 120;
            p->b = 120;
            p->u0 = 0;
            p->v0 = (taiiku_tama_gscrl_hi >> 1) + k;
            p->u1 = 64;
            p->v1 = (taiiku_tama_gscrl_hi >> 1) + k;
            p->u2 = 0;
            p->v2 = (taiiku_tama_gscrl_hi >> 1) + k + 8;
            p->u3 = 64;
            p->v3 = (taiiku_tama_gscrl_hi >> 1) + k + 8;
            break;
        default:
            p->r = 128;
            p->g = 128;
            p->b = 128;
            p->u0 = 0;
            p->v0 = taiiku_tama_gscrl_hi + k;
            p->u1 = 64;
            p->v1 = taiiku_tama_gscrl_hi + k;
            p->u2 = 0;
            p->v2 = taiiku_tama_gscrl_hi + k + 8;
            p->u3 = 64;
            p->v3 = taiiku_tama_gscrl_hi + k + 8;
            break;
        }

        p->x0 = taiiku_tama_gnd_x0[i] + sx;
        p->y0 = taiiku_tama_gnd_y0[i] + sy;
        p->x1 = taiiku_tama_gnd_x1[i] + sx;
        p->y1 = taiiku_tama_gnd_y0[i] + sy;
        p->x2 = taiiku_tama_gnd_x2[i] + sx;
        p->y2 = taiiku_tama_gnd_y1[i] + sy;
        p->x3 = taiiku_tama_gnd_x3[i] + sx;
        p->y3 = taiiku_tama_gnd_y1[i] + sy;
        p->tpage = GetTPage(1, 0, 768, 0);
        p->clut = GetClut(0, 496);
        AddPrim(&ot_buff0[activeBuff][251], p);
    }
}
