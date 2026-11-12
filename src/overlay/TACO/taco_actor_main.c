/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=1 */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic0 */
/* @PostProc: remap_stack_slots --from 72 --to 68 --from 74 --to 70 --from 76 --to 72 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0xa8 --offset2 0xb0 */
/* @PostProc: patch_insn_reg_field 0x44:rs:t6:s1 0x44:rt:s1:t6 0x7c:rt:t1:t2 0x80:rd:v0:t1 0x84:rs:v0:t1 0x88:rs:t1:t2 0x88:rt:t1:t2 0x90:rs:t1:s1 0x90:rt:s1:t2 0x94:rt:t2:t3 0x9c:rs:t2:t3 0xa4:rt:t3:t4 0xac:rt:t3:t4 0xb0:rt:t4:t6 0xb8:rs:t4:t6 0xb8:rt:t4:t6 0xbc:rt:t6:t8 0xc0:rd:a0:t7 0xc0:rs:t4:t6 0xc4:rs:a0:t7 0xc8:rs:t6:t8 0xc8:rt:t6:t8 0xcc:rt:t7:t0 0xd0:rd:v0:t9 0xd0:rs:t6:t8 0xd4:rs:v0:t9 0xd8:rs:t7:t0 0xd8:rt:t7:t0 0xe0:rs:t7:s1 0xe0:rt:s1:t0 0xe4:rt:t8:t1 0xec:rt:t8:t1 0xf0:rt:t9:t2 0xf8:rt:t9:t2 0xfc:rt:t0:t3 0x104:rt:t0:t3 0x110:rt:t1:t4 0x114:rs:t1:t4 0x11c:rs:t1:t4 0x124:rs:t1:t4 0x12c:rs:t1:t4 0x144:rt:t2:t6 0x15c:rs:t2:t6 0x164:rd:t2:t6 0x164:rt:t2:t6 0x16c:rt:t2:t6 0x170:rt:t2:t6 0x178:rs:t2:t6 0x194:rt:t4:t7 0x198:rt:t4:t7 0x1a0:rt:t4:t7 0x1a4:rt:t4:t7 0x1b4:rt:t5:t8 0x1b8:rs:t5:t8 0x1b8:rt:t5:t8 0x1c0:rs:t5:s1 0x1c0:rt:s1:t8 0x1d8:rt:t7:t0 0x1dc:rt:t7:t0 0x1e4:rt:t7:t0 0x1e8:rt:t7:t0 0x1f8:rt:t8:t1 0x1fc:rs:t8:t1 0x1fc:rt:t8:t1 0x204:rs:t8:s1 0x204:rt:s1:t1 0x21c:rt:t0:t3 0x220:rt:t0:t3 0x228:rt:t0:t3 0x22c:rt:t0:t3 0x23c:rt:t1:t4 0x240:rs:t1:t4 0x240:rt:t1:t4 0x248:rs:t1:s1 0x248:rt:s1:t4 0x260:rt:t3:t5 0x264:rt:t3:t5 0x26c:rt:t3:t5 0x270:rt:t3:t5 0x280:rt:t4:t7 0x284:rs:t4:t7 0x284:rt:t4:t7 0x28c:rs:t4:s1 0x28c:rt:s1:t7 0x2a4:rt:t6:t9 0x2a8:rt:t6:t9 0x2b0:rt:t6:t9 0x2b4:rt:t6:t9 0x2c4:rt:t7:t0 0x2c8:rs:t7:t0 0x2c8:rt:t7:t0 0x2d0:rs:t7:s1 0x2d0:rt:s1:t0 0x2e8:rt:t9:t2 0x2ec:rt:t9:t2 0x2f4:rt:t9:t2 0x2f8:rt:t9:t2 0x308:rt:t0:t3 0x30c:rs:t0:t3 0x30c:rt:t0:t3 0x314:rs:t0:s1 0x314:rt:s1:t3 0x32c:rt:t2:t6 0x330:rt:t2:t6 0x338:rt:t2:t6 0x33c:rt:t2:t6 0x34c:rt:t3:t5 0x350:rs:t3:t5 0x350:rt:t3:t5 0x358:rs:t3:s1 0x358:rt:s1:t5 0x370:rt:t5:t8 0x374:rt:t5:t8 0x37c:rt:t5:t8 0x380:rt:t5:t8 0x390:rt:t6:t9 0x394:rs:t6:t9 0x394:rt:t6:t9 0x39c:rs:t6:s1 0x39c:rt:s1:t9 0x3b4:rt:t8:t1 0x3b8:rt:t8:t1 0x3c0:rt:t8:t1 0x3c4:rt:t8:t1 0x3e8:rt:t0:t3 0x3ec:rt:t0:t3 0x3f4:rt:t0:t3 0x3f8:rt:t0:t3 0x41c:rt:t2:t6 0x420:rt:t2:t6 0x428:rt:t2:t6 0x42c:rt:t2:t6 0x450:rt:t4:t7 0x454:rt:t4:t7 0x45c:rt:t4:t7 0x460:rt:t4:t7 0x484:rt:t6:t9 0x488:rt:t6:t9 0x490:rt:t6:t9 0x494:rt:t6:t9 0x4a4:rt:t7:t0 0x4a8:rs:t7:t0 0x4a8:rt:t7:t0 0x4b0:rs:t7:s1 0x4b0:rt:s1:t0 0x4c8:rt:t9:t2 0x4cc:rt:t9:t2 0x4d4:rt:t9:t2 0x4d8:rt:t9:t2 0x4fc:rt:t1:t4 0x500:rt:t1:t4 0x508:rt:t1:t4 0x50c:rt:t1:t4 0x530:rt:t3:t5 0x534:rt:t3:t5 0x53c:rt:t3:t5 0x540:rt:t3:t5 0x550:rt:t4:t7 0x554:rs:t4:t7 0x554:rt:t4:t7 0x55c:rs:t4:s1 0x55c:rt:s1:t7 0x574:rt:t6:t9 0x578:rt:t6:t9 0x580:rt:t6:t9 0x584:rt:t6:t9 0x5a8:rt:t8:t1 0x5ac:rt:t8:t1 0x5b4:rt:t8:t1 0x5b8:rt:t8:t1 0x5dc:rt:t0:t3 0x5e0:rt:t0:t3 0x5e8:rt:t0:t3 0x5ec:rt:t0:t3 0x610:rt:t2:t6 0x614:rt:t2:t6 0x61c:rt:t2:t6 0x620:rt:t2:t6 0x644:rt:t4:t7 0x648:rt:t4:t7 0x650:rt:t4:t7 0x654:rt:t4:t7 0x678:rt:t6:t9 0x67c:rt:t6:t9 0x684:rt:t6:t9 0x688:rt:t6:t9 0x6ac:rt:t8:t1 0x6b0:rt:t8:t1 0x6b8:rt:t8:t1 0x6bc:rt:t8:t1 0x6e0:rt:t0:t3 0x6e4:rt:t0:t3 0x6ec:rt:t0:t3 0x6f0:rt:t0:t3 0x714:rt:t2:t6 0x718:rt:t2:t6 0x720:rt:t2:t6 0x724:rt:t2:t6 0x734:rt:t3:t5 0x738:rs:t3:t5 0x738:rt:t3:t5 0x740:rs:t3:s1 0x740:rt:s1:t5 0x758:rt:t4:t7 0x75c:rs:t4:t7 0x75c:rt:t4:t7 0x764:rs:t4:s1 0x764:rt:s1:t7 0x768:rt:t5:t8 0x770:rs:t5:t8 0x770:rt:t6:t9 0x774:rt:t6:t9 */
/* @RodataBase: 0x8015E000 */
/* Address: 0x8015B5AC */
#include "types.h"

typedef struct {
    u8  pad00;
    u8  f01;
    u8  f02;
    u8  f03;
    u8  pad04[112];
    s16 f116;
    s16 f118;
    s16 f120;
    s16 f122;
    u8  pad7C[9];
    u8  f133;
    u8  pad86;
    u8  f135;
} Entry;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 pad;
} Vec3;

extern volatile s32 taco_actor_rec;

extern s32 taco_item_add(Vec3 v, s32 kind);
extern void taco_actor_disp_switch(s32 a, s32 b);
extern void taco_actor_dispatch(s32 a, s32 b);
extern s32 func_8015C2DC(s32 a);

void
taco_actor_main(void)
{
    Vec3 sv;
    Entry *p;
    Entry *q;
    s32 s1;
    s32 s2;
    s32 s3;
    s32 v1;

    s1 = 2176;
    s2 = 16;
    s3 = 255;
    do {
        p = (Entry *)(taco_actor_rec + s1);
        if ((p->f01 & 0x80) == 0) {
            goto next;
        }
        if (p->f122 == 32767) {
            taco_actor_disp_switch(1, s2);
            p = (Entry *)(taco_actor_rec + s1);
            p->f122 = 0;
            p = (Entry *)(taco_actor_rec + s1);
        }
        if (p->f133 == 0 && p->f02 != s3) {
        p->f122 = 0;
        q = (Entry *)(taco_actor_rec + s1);
        q->f03 = 2;
        p = (Entry *)(taco_actor_rec + s1);
        p->f02 = s3;
        p = (Entry *)(taco_actor_rec + s1);
        sv.x = p->f116;
        sv.y = p->f118;
        sv.z = p->f120;
        v1 = p->f135 & 0xF0;
        switch ((u32)v1) {
        case 16:
            taco_item_add(sv, 6);
            p = (Entry *)(taco_actor_rec + s1);
            break;
        case 32:
            taco_item_add(sv, 7);
            p = (Entry *)(taco_actor_rec + s1);
            break;
        case 48:
            taco_item_add(sv, 0);
            p = (Entry *)(taco_actor_rec + s1);
            break;
        case 64:
            taco_item_add(sv, 1);
            p = (Entry *)(taco_actor_rec + s1);
            break;
        case 80:
            taco_item_add(sv, 2);
            p = (Entry *)(taco_actor_rec + s1);
            break;
        case 96:
            taco_item_add(sv, 3);
            p = (Entry *)(taco_actor_rec + s1);
            break;
        case 112:
            taco_item_add(sv, 4);
            p = (Entry *)(taco_actor_rec + s1);
            break;
        case 128:
            taco_item_add(sv, 5);
            p = (Entry *)(taco_actor_rec + s1);
            break;
        case 208:
            taco_item_add(sv, 6);
            taco_item_add(sv, 6);
            taco_item_add(sv, 6);
            taco_item_add(sv, 6);
            taco_item_add(sv, 6);
            p = (Entry *)(taco_actor_rec + s1);
            break;
        case 224:
            taco_item_add(sv, 0);
            taco_item_add(sv, 1);
            taco_item_add(sv, 7);
            p = (Entry *)(taco_actor_rec + s1);
            break;
        case 240:
            taco_item_add(sv, 6);
            taco_item_add(sv, 6);
            taco_item_add(sv, 7);
            taco_item_add(sv, 2);
            taco_item_add(sv, 0);
            taco_item_add(sv, 1);
            taco_item_add(sv, 5);
            taco_item_add(sv, 3);
            taco_item_add(sv, 4);
            p = (Entry *)(taco_actor_rec + s1);
            break;
        }
        }
    tail:
        taco_actor_dispatch(p->f02, s2);
        func_8015C2DC(s2);
        p = (Entry *)(taco_actor_rec + s1);
        p->f122 = p->f122 + 1;
    next:
        s2 += 1;
        s1 += 136;
    } while (s2 != 63);
}
