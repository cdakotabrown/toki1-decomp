/* @PostProc: patch_stack_complete --old 80 --new 96 */
/* @PostProc: remap_stack_slots --map '50:65,51:66' */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x3c */
/* @PostProc: swap_instructions --offset1 0x90 --offset2 0xa0 */
/* @PostProc: swap_instructions --offset1 0x2cc --offset2 0x2d0 */
/* @PostProc: swap_instructions --offset1 0x348 --offset2 0x34c */
/* @PostProc: swap_instructions --offset1 0x3ac --offset2 0x3b0 */
/* @PostProc: swap_instructions --offset1 0x190 --offset2 0x198 */
/* @PostProc: swap_instructions --offset1 0x194 --offset2 0x1c0 */
/* @PostProc: swap_instructions --offset1 0x198 --offset2 0x1c4 */
/* @PostProc: swap_instructions --offset1 0x19c --offset2 0x1c8 */
/* @PostProc: swap_instructions --offset1 0x1a0 --offset2 0x1cc */
/* @PostProc: swap_instructions --offset1 0x1a4 --offset2 0x1d0 */
/* @PostProc: swap_instructions --offset1 0x1a8 --offset2 0x1d4 */
/* @PostProc: swap_instructions --offset1 0x1ac --offset2 0x1c4 */
/* @PostProc: swap_instructions --offset1 0x1b0 --offset2 0x1c0 */
/* @PostProc: swap_instructions --offset1 0x1b4 --offset2 0x1cc */
/* @PostProc: swap_instructions --offset1 0x1b8 --offset2 0x1c8 */
/* @PostProc: swap_instructions --offset1 0x1bc --offset2 0x1c0 */
/* @PostProc: swap_instructions --offset1 0x1c0 --offset2 0x1d0 */
/* @PostProc: swap_instructions --offset1 0x1c4 --offset2 0x1d4 */
/* @PostProc: swap_instructions --offset1 0x1c8 --offset2 0x1d4 */
/* @PostProc: swap_instructions --offset1 0x1d0 --offset2 0x1d4 */
/* @PostProc: swap_instructions --offset1 0x1fc --offset2 0x204 */
/* @PostProc: swap_instructions --offset1 0x200 --offset2 0x208 */
/* @PostProc: swap_instructions --offset1 0x204 --offset2 0x20c */
/* @PostProc: swap_instructions --offset1 0x208 --offset2 0x20c */
/* @PostProc: swap_instructions --offset1 0x290 --offset2 0x2a0 */
/* @PostProc: swap_instructions --offset1 0x29c --offset2 0x2a4 */
/* @PostProc: swap_instructions --offset1 0x2a8 --offset2 0x2b0 */
/* @PostProc: swap_instructions --offset1 0x2ac --offset2 0x2b0 */
/* @PostProc: swap_instructions --offset1 0x2b0 --offset2 0x2b4 */
/* @PostProc: swap_instructions --offset1 0x2b4 --offset2 0x2bc */
/* @PostProc: swap_instructions --offset1 0x2b8 --offset2 0x2bc */
/* @PostProc: patch_insn_reg_field 0x38:rt:t7:t6 0x3c:rt:t6:t7 0x40:rt:t6:t7 0x44:rt:t7:t6 0x78:rs:t9:v0 0x78:rt:v0:t9 0x98:rs:t6:v0 0x98:rt:v0:t6 0xc8:rt:t5:t4 0xcc:rt:t8:t7 0xd0:rd:t6:t5 0xd0:rt:t5:t4 0xd4:rs:t6:t5 0xd4:rt:t7:t6 0xd8:rs:t8:t7 0xd8:rt:t8:t7 0xdc:rs:t7:t6 0xdc:rt:t8:t7 0x100:rt:t9:t8 0x104:rs:t9:t8 0x104:rt:t9:t8 0x10c:rs:t9:t8 0x170:rd:t2:t9 0x174:rd:t2:t9 0x174:rs:t2:t9 0x178:rt:t2:t9 0x184:rt:t6:t2 0x188:rs:t6:t2 0x198:rd:t7:t3 0x1a0:rd:t7:t3 0x1a4:rs:t7:t3 0x1ac:rt:t3:t7 0x1b0:rs:t3:t7 0x1b0:rt:t3:t7 0x1b4:rt:t5:t9 0x1b8:rs:t3:t7 0x1bc:rt:t5:t9 0x1c4:rd:t4:t8 0x1c8:rs:t4:t8 0x1d8:rd:t8:s0 0x1dc:rs:t8:s0 0x1dc:rt:t9:t4 0x1e0:rt:t9:t4 0x1e8:rd:t3:t0 0x1ec:rs:t3:t0 0x1ec:rt:t4:t5 0x1f0:rd:t5:t3 0x1f0:rt:t4:t5 0x1f8:rd:t6:t4 0x1f8:rt:t5:t3 0x1fc:rd:t7:t1 0x200:rs:t7:t1 0x200:rt:t8:t6 0x204:rd:t9:t5 0x204:rs:t6:t4 0x204:rt:t8:t6 0x208:rt:t3:t6 0x20c:rt:t3:t6 0x214:rt:t9:t5 0x264:rt:v1:v0 0x268:rt:v1:v0 0x26c:rt:v1:v0 0x270:rt:v1:v0 0x274:rt:t4:t7 0x278:rs:t4:t7 0x278:rt:t4:t7 0x27c:rt:v0:v1 0x280:rs:v0:v1 0x280:rt:v0:v1 0x284:rd:t2:t8 0x284:rt:t4:t7 0x288:rd:t5:t9 0x288:rs:t2:v1 0x288:rt:v0:t8 0x28c:rs:t5:t9 0x28c:rt:t7:t2 0x290:rt:t5:t4 0x294:rs:t7:t2 0x294:rt:t6:t3 0x298:rt:t6:t3 0x29c:rs:t5:t4 0x29c:rt:t5:t4 0x2a8:rd:t7:t5 0x2a8:rt:t5:t4 0x2b0:rs:v0:v1 0x2b0:rt:t7:t5 0x2b4:rt:t8:t7 0x2b8:rt:t3:t2 0x2bc:rd:t4:t3 0x2bc:rs:t3:t2 0x2c0:rd:t2:t4 0x2c0:rt:t4:t3 0x2c4:rd:t3:t5 0x2c4:rs:t2:t7 0x2c4:rt:t8:t4 0x2c8:rs:t3:t5 0x2c8:rt:t9:t6 0x2cc:rt:t5:t2 0x2d0:rt:t4:t8 0x2d4:rt:t9:t6 0x2d8:rt:t4:t8 0x2dc:rt:t5:t2 0x2f0:rt:t7:t9 0x2f4:rs:t7:t9 0x2f4:rt:t6:t9 0x2f8:rt:t3:t4 0x2fc:rd:t2:t3 0x2fc:rt:t6:t9 0x300:rs:t2:t3 0x300:rt:t8:t7 0x304:rs:t3:t4 0x304:rt:t3:t4 0x308:rs:t8:t7 0x308:rt:t3:t4 0x348:rt:t4:t5 0x34c:rt:t9:t6 0x350:rt:t9:t6 0x354:rt:t4:t5 0x374:rt:t5:t8 0x378:rs:t5:t8 0x378:rt:t5:t8 0x37c:rt:t6:t9 0x380:rd:t7:t2 0x380:rt:t5:t8 0x384:rd:t6:t9 0x384:rs:t6:t9 0x384:rt:t7:t2 0x388:rs:t6:t9 0x388:rt:t6:t9 0x38c:rt:t8:t7 0x390:rs:t6:t9 0x390:rt:t2:t3 0x394:rt:t2:t3 0x398:rs:t8:t7 0x398:rt:t8:t7 0x39c:rt:t9:t5 0x3a0:rd:t3:t4 0x3a0:rt:t8:t7 0x3a4:rd:t9:t5 0x3a4:rs:t9:t5 0x3a4:rt:t3:t4 0x3a8:rs:t9:t5 0x3a8:rt:t9:t5 0x3ac:rt:t7:t2 0x3b0:rt:t5:t8 0x3b4:rs:t9:t5 0x3b4:rt:t4:t6 0x3b8:rt:t4:t6 0x3bc:rt:t5:t8 0x3c0:rt:t7:t2 0x3d4:rt:t6:t9 0x3d8:rs:t6:t9 0x3d8:rt:t2:t9 0x3dc:rt:t9:t4 0x3e0:rd:t8:t3 0x3e0:rt:t2:t9 0x3e4:rs:t8:t3 0x3e4:rt:t3:t7 0x3e8:rs:t9:t4 0x3e8:rt:t9:t4 0x3ec:rs:t3:t7 0x3ec:rt:t9:t4 */
/* Address: 0x8013FB00 */
/* Size: 1040 bytes (260 insns) */
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
extern s32 frame_counter;
extern u8 screen_w[];
extern u8 D_800EE760_b[];
extern s16 disp_ofs_x[];
extern s16 disp_ofs_y[];
extern u32 ot_buff0[];
extern u8 *taco_actor_rec;

extern Sprt *prim_alloc(s32, s32);
extern void dtd_on_tpage(s32, s32, s32, s32, s32);
extern void _sprite_set_box_shade(s32, s32, s32, s32, s32, s32, s32);
extern s32 rsin(s32);
extern u16 GetClut(s32, s32);
extern void AddPrim(u32 *, Sprt *);
extern void SetSemiTrans(Sprt *, s32);
extern void SetSprt(Sprt *);

void taco_gauge_disp(void)
{
    Sprt *p;
    u8 *base;
    u8 *q;
    s32 v;
    s32 lvl;
    s32 c;
    s32 r;
    u8 g;
    u8 b;

    p = prim_alloc(20, activeBuff);
    SetSprt(p);
    p->h = 64;
    p->w = 32;
    SetSemiTrans(p, 1);
    p->r0 = 255;
    p->g0 = 255;
    p->b0 = 255;
    base = (u8 *)((u32)screen_w | 0);
    p->x0 = (s16)(*(s16 *)(base + activeBuff * 2 + 20) - 200);
    p->y0 = (s16)(*(s16 *)(base + activeBuff * 2 + 24) + 40);
    p->u0 = 128;
    p->v0 = 64;
    p->clut = GetClut(16, 495);
    AddPrim(&ot_buff0[activeBuff * 256 + 1], p);
    dtd_on_tpage(832, 256, 1, 0, 0);

    v = taco_actor_rec[133];
    if ((u32)v >= 193U) {
        r = 0;
        g = 0;
        b = 1;
    } else if ((u32)v >= 129U) {
        r = 0;
        g = 1;
        b = 0;
    } else if ((u32)v >= 65U) {
        r = 1;
        g = 1;
        b = 0;
    } else {
        r = 1;
        g = 0;
        b = 0;
    }
    lvl = (4096 - rsin(frame_counter * 240)) / 128 + 64;
    c = ((((r * lvl) & 0xFF) << 16) + (((g * lvl) & 0xFF) << 8)) +
        ((b * lvl) & 0xFF);
    q = taco_actor_rec;
    _sprite_set_box_shade(-191, 104, 14, (s32)(((u32)(0 - (s32)q[133])) >> 2), 2, c,
                  129);
    dtd_on_tpage(0, 0, 2, 1, 0);

    p = prim_alloc(20, activeBuff);
    SetSprt(p);
    p->w = 32;
    p->h = 32;
    SetSemiTrans(p, 1);
    p->r0 = 128;
    p->g0 = 128;
    p->b0 = 128;
    base = (u8 *)((u32)D_800EE760_b | 0);
    p->x0 = (s16)(*(s16 *)(base + activeBuff * 2 + 20) - 200);
    q = taco_actor_rec;
    p->y0 = (s16)((*(s16 *)(base + activeBuff * 2 + 24) +
                   (s32)(((u32)(255 - q[133])) >> 2)) + 24);
    p->u0 = 160;
    p->v0 = 96;
    p->clut = GetClut(32, 495);
    AddPrim(&ot_buff0[activeBuff * 256 + 1], p);
    dtd_on_tpage(832, 256, 1, 0, 0);

    p = prim_alloc(20, activeBuff);
    SetSprt(p);
    p->h = 24;
    p->w = 40;
    SetSemiTrans(p, 1);
    p->r0 = 128;
    p->g0 = 128;
    p->b0 = 128;
    p->x0 = (s16)(disp_ofs_x[activeBuff] - 206);
    p->y0 = (s16)(disp_ofs_y[activeBuff] + 104);
    p->u0 = 160;
    p->v0 = 64;
    p->clut = GetClut(32, 495);
    AddPrim(&ot_buff0[activeBuff * 256 + 1], p);
    dtd_on_tpage(832, 256, 1, 0, 0);
}
