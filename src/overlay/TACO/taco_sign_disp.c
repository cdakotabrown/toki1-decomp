/* @PostProc: swap_instructions --offset1 0x1a0 --offset2 0x1a4 */
/* @PostProc: patch_insn_reg_field 0x50:rt:a2:v1 0x54:rt:a2:v1 0x58:rt:a2:v1 0x5c:rt:a2:v1 0x68:rt:v0:a2 0x6c:rs:v0:a2 0x6c:rt:v0:a2 0x78:rs:t0:a2 0x78:rt:v0:t0 0x84:rt:v1:v0 0x98:rs:v1:v0 0xa4:rs:t5:a2 0xa4:rt:v0:t5 0x104:rt:t4:t3 0x108:rt:t7:t6 0x10c:rd:t5:t4 0x10c:rt:t4:t3 0x110:rs:t5:t4 0x110:rt:t6:t5 0x114:rs:t7:t6 0x114:rt:t7:t6 0x118:rs:t6:t5 0x118:rt:t7:t6 0x168:rt:t8:t7 0x16c:rs:t8:t7 0x16c:rt:t8:t7 0x170:rt:t0:t9 0x174:rd:t9:t8 0x174:rt:t8:t7 0x178:rd:t0:t9 0x178:rs:t0:t9 0x178:rt:t9:t8 0x17c:rs:t0:t9 0x17c:rt:t0:t9 0x180:rt:t2:t1 0x184:rs:t0:t9 0x184:rt:t1:t0 0x188:rt:t1:t0 0x18c:rs:t2:t1 0x18c:rt:t2:t1 0x190:rt:t4:t3 0x194:rd:t3:t2 0x194:rt:t2:t1 0x198:rd:t4:t3 0x198:rs:t4:t3 0x198:rt:t3:t2 0x19c:rs:t4:t3 0x19c:rt:t4:t3 0x1a0:rt:t7:t5 0x1a8:rs:t4:t3 0x1a8:rt:t5:t4 0x1ac:rt:t5:t4 0x1b4:rt:t7:t5 0x1b8:rt:t8:t7 0x1bc:rs:t8:t7 0x1bc:rt:t8:t7 0x1c4:rs:t8:t7 0x1c4:rt:t9:t8 0x1cc:rs:t9:t8 0x1f4:rt:t0:t9 0x1f8:rs:t0:t9 0x1f8:rt:t1:t9 0x1fc:rt:t4:t2 0x200:rd:t2:t0 0x200:rt:t1:t9 0x204:rs:t2:t0 0x204:rt:t3:t1 0x208:rs:t4:t2 0x208:rt:t4:t2 0x20c:rs:t3:t1 0x20c:rt:t4:t2 */
/* Address: 0x8013F8D0 */
/* Size: 560 bytes (140 insns) */
#include "types.h"

typedef struct
{
    u32 tag;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
    s16 x0;
    s16 y0;
    u8 u0;
    u8 v0;
    u16 clut;
    s16 w;
    s16 h;
} Sprt;

extern s32 activeBuff;
extern u8 screen_w[];
extern s16 disp_ofs_x[];
extern s16 disp_ofs_y[];
extern u32 ot_buff0[];
extern u8 *taco_actor_rec;

extern Sprt *prim_alloc(s32, s32);
extern void dtd_on_tpage(s32, s32, s32, s32, s32);
extern u16 GetClut(s32, s32);
extern void AddPrim(u32 *, Sprt *);
extern void SetSemiTrans(Sprt *, s32);
extern void SetSprt(Sprt *);

void taco_sign_disp(void)
{
    Sprt *p;
    u8 *base;
    u32 v;

    p = prim_alloc(20, activeBuff);
    SetSprt(p);
    p->w = 32;
    p->h = 32;
    SetSemiTrans(p, 1);
    p->r0 = 128;
    p->g0 = 128;
    p->b0 = 128;

    base = (u8 *)((u32)screen_w | 0);
    v = ((u32)(taco_actor_rec[2] & 0x40)) >> 2;
    p->x0 = (s16)(*(s16 *)(base + activeBuff * 2 + 20) + 200);
    p->y0 = (s16)((*(s16 *)(base + activeBuff * 2 + 24) + v) + 40);
    p->u0 = 224;
    p->v0 = 0;
    if ((u32)taco_actor_rec[133] < 32U) {
        p->clut = GetClut(16, 495);
    } else {
        p->clut = GetClut(0, 495);
    }
    AddPrim(&ot_buff0[activeBuff * 256 + 1], p);

    p = prim_alloc(20, activeBuff);
    SetSprt(p);
    p->w = 32;
    p->h = 32;
    SetSemiTrans(p, 1);
    p->r0 = 128;
    p->g0 = 128;
    p->b0 = 128;
    p->x0 = (s16)(disp_ofs_x[activeBuff] + 200);
    p->y0 = (s16)(disp_ofs_y[activeBuff] + 72);
    p->v0 = 32;
    p->u0 = 224;
    if ((u32)taco_actor_rec[133] < 32U) {
        p->clut = GetClut(16, 495);
    } else {
        p->clut = GetClut(0, 495);
    }
    AddPrim(&ot_buff0[activeBuff * 256 + 1], p);

    dtd_on_tpage(832, 256, 1, 0, 0);
}
