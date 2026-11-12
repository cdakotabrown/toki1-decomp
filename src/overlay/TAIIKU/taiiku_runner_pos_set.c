/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=4 */
/* @PostProc: patch_insn_reg_field 0x1b8:rd:t0:t2 0x1bc:rt:t1:t3 0x1c0:rd:t2:t4 0x1c0:rs:t1:t3 0x1c0:rt:t0:t2 0x1c4:rt:t2:t4 0x1cc:rd:t3:t0 0x1d4:rs:t3:t0 0x1d4:rt:t4:t1 0x1dc:rt:t4:t1 0x1f0:rt:t9:t0 0x1f4:rs:t9:t0 0x1fc:rt:t2:t3 0x200:rt:t0:t9 0x204:rd:t3:t2 0x204:rs:t2:t3 0x208:rs:t3:t2 0x20c:rt:t0:t9 0x224:rt:t1:t0 0x26c:rt:t0:t1 0x270:rt:t1:t0 0x278:rt:t0:t1 0x27c:rt:t2:t3 0x284:rs:t2:t3 0x284:rt:t3:t2 0x28c:rt:t3:t2 0x2b8:rt:t7:t9 0x2bc:rd:t8:t0 0x2bc:rs:t7:t9 0x2c0:rt:t8:t0 0x2c4:rt:t0:t1 0x2c8:rt:t9:t7 0x2cc:rd:t2:t3 0x2cc:rs:t0:t1 0x2d0:rd:t1:t8 0x2d0:rs:t9:t7 0x2d4:rs:t2:t3 0x2d4:rt:t3:t2 0x2d8:rt:t1:t8 0x2e0:rt:t3:t2 0x2e8:rd:t4:t7 0x2ec:rt:t5:t8 0x2f0:rd:t6:t9 0x2f0:rs:t5:t8 0x2f0:rt:t4:t7 0x2f4:rt:t6:t9 0x2f8:rt:t1:t0 0x2fc:rd:t7:t4 0x300:rt:t8:t5 0x304:rd:t0:t1 0x304:rs:t1:t0 0x308:rd:t9:t6 0x308:rs:t8:t5 0x308:rt:t7:t4 0x30c:rs:t0:t1 0x30c:rt:t2:t3 0x310:rt:t9:t6 0x314:rt:t2:t3 0x32c:rt:t3:t2 0x334:rs:t3:t2 0x380:rt:t1:t0 0x388:rs:t1:t0 0x388:rt:t0:t1 0x390:rt:t0:t1 0x394:rd:t2:t3 0x3a0:rd:t2:t3 0x3a0:rs:t2:t3 0x3a4:rt:t3:t2 0x3b0:rs:t3:t2 0x3b0:rt:t3:t2 0x3b4:rd:t2:t3 0x3b4:rt:t2:t3 0x3b8:rs:t2:t3 0x3b8:rt:t3:t2 0x3c8:rt:t0:t1 0x3cc:rt:t1:t0 0x3d0:rt:t1:t0 0x3d8:rt:t0:t1 0x3dc:rd:t2:t3 0x3e0:rd:t2:t3 0x3e0:rs:t2:t3 0x3e8:rt:t3:t2 0x3ec:rs:t3:t2 0x3ec:rt:t3:t2 0x3f4:rd:t2:t3 0x3f4:rt:t2:t3 0x3fc:rs:t2:t3 0x3fc:rt:t3:t2 0x400:rd:t5:t8 0x404:rs:t5:t8 0x404:rt:t4:t7 0x408:rt:t4:t7 0x410:rd:t6:t5 0x414:rt:t8:t4 0x418:rd:t1:t0 0x41c:rd:t7:t6 0x41c:rs:t8:t4 0x41c:rt:t6:t5 0x420:rs:t1:t0 0x420:rt:t0:t1 0x424:rt:t7:t6 0x42c:rt:t0:t1 0x434:rd:t2:t3 0x438:rs:t2:t3 0x438:rt:t3:t5 0x43c:rt:t3:t5 0x440:rt:t8:t6 0x444:rt:t5:t2 0x448:rd:t6:t8 0x448:rs:t8:t6 0x44c:rs:t5:t2 0x44c:rt:t2:t3 0x450:rs:t6:t8 0x46c:rt:t2:t3 0x490:rt:t1:t0 0x498:rs:t1:t0 0x498:rt:t0:t1 0x4a0:rt:t0:t1 0x4a4:rt:t3:t2 0x4a8:rt:t2:t3 0x4b0:rt:t3:t2 0x4b4:rd:t5:t4 0x4b8:rd:t5:t4 0x4b8:rs:t5:t4 0x4c0:rt:t4:t5 0x4c4:rs:t4:t5 0x4c4:rt:t4:t5 0x4cc:rd:t5:t4 0x4cc:rt:t5:t4 0x4d4:rs:t5:t4 0x4d4:rt:t4:t5 0x4d8:rd:t8:t7 0x4dc:rs:t8:t7 0x4dc:rt:t6:t9 0x4e0:rt:t6:t9 0x4e4:rt:t1:t0 0x4e8:rd:t7:t6 0x4ec:rd:t0:t1 0x4ec:rs:t1:t0 0x4f0:rs:t7:t6 0x4f0:rt:t9:t8 0x4f4:rs:t0:t1 0x4f4:rt:t2:t3 0x4f8:rt:t9:t8 0x500:rt:t2:t3 0x508:rd:t3:t5 0x50c:rs:t3:t5 0x50c:rt:t5:t6 0x510:rt:t5:t6 0x514:rt:t6:t8 0x518:rd:t4:t2 0x51c:rs:t6:t8 0x520:rs:t4:t2 0x520:rt:t8:t4 0x528:rt:t8:t4 0x540:rt:t6:t8 0x544:rt:t1:t0 0x548:rt:t0:t1 0x54c:rs:t1:t0 0x550:rt:t2:t3 0x554:rs:t2:t3 0x554:rt:t2:t3 0x558:rt:t2:t3 0x55c:rt:t0:t1 0x560:rt:t3:t2 0x56c:rt:t3:t2 0x570:rt:t5:t4 0x574:rt:t5:t4 0x584:rt:t4:t5 0x58c:rs:t4:t5 0x58c:rt:t8:t6 0x594:rt:t8:t6 0x59c:rt:t6:t8 */
/* @RodataBase: 0x80148E54 */
/* Address: 0x8013F57C */
/* Size: 1444 bytes (361 insns) */
#include "types.h"

typedef struct {
    u8  pad0[3];
    u8  f3;
    s32 pad1[5];
    s16 f24;
    s16 pad2[6];
    s16 f38;
    s16 pad3;
    s16 f42;
    s32 pad4[6];
} Rec68;

extern Rec68 class_w[];
extern s32 taiiku_course_no;
extern s32 taiiku_tama_fall;
extern s16 taiiku_tama_dist_hi;
extern u8 taiiku_runner_vis_tbl[];

extern s32 get_taiiku_seg(s32 idx);
extern s32 get_taiiku_seg_dist(s32 idx);

void taiiku_runner_pos_set(s32 idx)
{
    Rec68 *p;
    s32 mode;
    s32 val;
    s32 v;
    s32 t;

    if (idx == 0) {
        t = taiiku_tama_fall;
        mode = taiiku_course_no;
        val = taiiku_tama_dist_hi;
        if (t != 0) {
            class_w[104].f3 |= 0x40;
            class_w[108].f3 |= 0x40;
        } else {
            class_w[104].f3 &= ~0x40;
            class_w[108].f3 &= ~0x40;
        }
    } else {
        if (taiiku_runner_vis_tbl[idx] == 1) {
            p = &class_w[idx];
            p[104].f3 |= 0x40;
            p[108].f3 |= 0x40;
        } else {
            p = &class_w[idx];
            p[104].f3 &= ~0x40;
            p[108].f3 &= ~0x40;
        }
        mode = get_taiiku_seg(idx);
        val = (s16)get_taiiku_seg_dist(idx);
    }

    switch (mode) {
    case 0:
    case 6:
        p = &class_w[idx];
        p[104].f38 = (val >> 3) + (val >> 6) - 72;
        p[108].f38 = p[104].f38 + 16;
        break;

    case 1:
        p = &class_w[idx];
        v = val - 512;
        if (v < 128) {
            p[104].f42 = -72 - (v >> 4);
            p[104].f38 = (v >> 2) + 72;
            p[104].f42 = p[104].f42 - idx + 3;
        } else {
            v = val - 640;
            p[104].f42 = -80 - (v >> 3);
            p[104].f38 = (v >> 3) + 104;
            p[104].f42 = p[104].f42 - idx + 3;
        }
        v = p[104].f38;
        if (v >= 104) {
            if (p[104].f24 < 142) {
                if (p == class_w) {
                    class_w[104].f24 = 144;
                } else {
                    p[104].f24 = 142;
                }
                p[104].f3 = 132;
            }
        }
        if (v < 104) {
            p[108].f38 = v + 16;
            p[108].f42 = p[104].f42;
        } else {
            p[108].f38 = v;
            p[108].f42 = p[104].f42 - 8;
        }
        break;

    case 2:
        p = &class_w[idx];
        if (val < 896) {
            v = val - 768;
            p[104].f42 = -96 - (v >> 4);
            p[104].f38 = 120 - (v >> 4);
            p[104].f42 = p[104].f42 - idx + 3;
        } else {
            v = val - 896;
            p[104].f42 = -104 - (v >> 4);
            p[104].f38 = 104 - (v >> 2);
            p[104].f42 = p[104].f42 - idx + 3;
        }
        v = p[104].f38;
        if (v < 105) {
            if (p[104].f24 >= 142) {
                if (p == class_w) {
                    class_w[104].f24 = 138;
                } else {
                    p[104].f24 = 134;
                }
                p[104].f3 = 132;
            }
        }
        if (v < 104) {
            p[108].f38 = v - 16;
            p[108].f42 = p[104].f42;
        } else {
            p[108].f38 = v;
            p[108].f42 = p[104].f42 - 8;
        }
        break;

    case 3:
        p = &class_w[idx];
        p[104].f38 = -(val >> 3) - (val >> 6) + 72;
        p[108].f38 = p[104].f38 - 16;
        p[108].f42 = p[104].f42;
        break;

    case 4:
        p = &class_w[idx];
        v = val - 512;
        if (v < 128) {
            p[104].f42 = (v >> 4) - 112;
            p[104].f38 = -72 - (v >> 2);
            p[104].f42 = p[104].f42 - idx + 3;
        } else {
            v = val - 640;
            p[104].f42 = (v >> 3) - 104;
            p[104].f38 = -104 - (v >> 3);
            p[104].f42 = p[104].f42 - idx + 3;
        }
        v = p[104].f38;
        if (v < -104) {
            if (p[104].f3 != 0) {
                p[104].f3 = 0;
            }
        }
        if (v < -104) {
            p[108].f38 = v;
            p[108].f42 = p[104].f42 + 8;
        } else {
            p[108].f38 = v - 16;
            p[108].f42 = p[104].f42;
        }
        break;

    case 5:
        p = &class_w[idx];
        v = val - 768;
        if (v < 128) {
            p[104].f42 = (v >> 4) - 88;
            p[104].f38 = (v >> 3) - 120;
            p[104].f42 = p[104].f42 - idx + 3;
        } else {
            v = val - 896;
            p[104].f42 = (v >> 4) - 80;
            p[104].f38 = (v >> 2) - 104;
            p[104].f42 = p[104].f42 - idx + 3;
        }
        v = p[104].f38;
        if (v >= -103) {
            if (p[104].f3 == 0) {
                p[104].f3 = 133;
                if (p == class_w) {
                    class_w[104].f24 = 138;
                } else {
                    p[104].f24 = 134;
                }
            }
        }
        if (v < -104) {
            p[108].f38 = v;
            p[108].f42 = p[104].f42 + 8;
        } else {
            p[108].f38 = v + 16;
            p[108].f42 = p[104].f42;
        }
        break;
    }
}
