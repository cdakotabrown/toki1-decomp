/* Function: parameter_up_down */
/* Address: 0x80074890 */
/* Size: 1228 bytes (307 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -cckr -Wc,-pic1 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --map 84:76 */
/* @PostProc: patch_insn_reg_field 0x4:rt:v0:v1 0x8:rs:v0:v1 0x8:rt:v0:v1 0x20:rs:v0:v1 0xb0:rt:t6:v0 0xb4:rs:t6:v0 0xb4:rt:t6:v0 0xb8:rt:t9:t8 0xbc:rt:t6:v0 0xbc:rd:t7:t6 0xc0:rt:t7:t6 0xc0:rd:t8:t7 0xc4:rs:t8:t7 0xc4:rt:t7:t6 0xc4:rd:t8:t7 0xc8:rs:t9:t8 0xc8:rt:t9:t8 0xcc:rs:t8:t7 0xcc:rt:t9:t8 0xcc:rd:t0:t9 0xd0:rt:t0:t9 0xd4:rs:t0:t9 0xe4:rd:t1:t0 0xf0:rs:t1:t0 0x110:rs:t1:t0 0x11c:rd:t2:t1 0x128:rs:t2:t1 0x144:rs:t2:t1 0x150:rd:t3:t2 0x15c:rs:t3:t2 0x178:rs:t3:t2 0x184:rd:t4:t3 0x190:rt:t4:t3 0x1bc:rt:t5:t4 0x1c0:rd:t6:t5 0x1c4:rs:t5:t4 0x1d4:rs:t6:t5 0x1d8:rd:t1:t0 0x1f8:rs:t6:t5 0x204:rd:t7:t6 0x210:rs:t7:t6 0x22c:rs:t7:t6 0x238:rd:t8:t7 0x244:rs:t8:t7 0x260:rs:t8:t7 0x270:rd:t9:t8 0x27c:rt:t9:t8 0x280:rd:t0:t9 0x284:rs:t0:t9 0x284:rt:t1:t0 0x298:rd:t2:t1 0x29c:rd:t3:t2 0x2a0:rs:t2:t1 0x2a0:rt:t3:t2 0x2b8:rd:t4:t3 0x2bc:rt:t5:t4 0x2c0:rs:t5:t4 0x2c0:rt:t5:t4 0x2c4:rs:t4:t3 0x2c4:rd:t4:t3 0x2c8:rs:t4:t3 0x2c8:rt:t5:t4 0x2c8:rd:t6:t5 0x2cc:rt:t6:t5 0x2d0:rs:t6:t5 0x2d8:rd:t7:t6 0x2e4:rs:t7:t6 0x304:rs:t7:t6 0x310:rd:t8:t7 0x31c:rs:t8:t7 0x338:rs:t8:t7 0x344:rd:t9:t8 0x350:rs:t9:t8 0x36c:rs:t9:t8 0x378:rd:t0:t9 0x384:rt:t0:t9 0x3b0:rt:t1:t0 0x3b4:rd:t2:t1 0x3b8:rs:t1:t0 0x3c8:rs:t2:t1 0x3cc:rd:t7:t6 0x3ec:rs:t2:t1 0x3f8:rd:t3:t2 0x404:rs:t3:t2 0x420:rs:t3:t2 0x42c:rd:t4:t3 0x438:rs:t4:t3 0x454:rs:t4:t3 0x464:rd:t5:t4 0x470:rt:t5:t4 0x474:rd:t6:t5 0x478:rs:t6:t5 0x478:rt:t7:t6 0x48c:rd:t8:t7 0x490:rd:t9:t8 0x494:rs:t8:t7 0x494:rt:t9:t8 */
#include "common.h"

extern u8 byouki_kind;
extern u16 basyo_pack;
extern s8 parameter_up_down_tbl1[];
extern s8 parameter_up_down_tbl0[];

void parameter_up_down(a0, a1, a2)
s32 a0;
s32 a1;
s32 a2;
{
    s32 xdiv;
    s32 ydiv;
    s32 i;
    s32 m;

    xdiv = 1;
    ydiv = 1;
    m = byouki_kind;
    if ((m & 0xf) == 3) {
        xdiv = 2;
    } else if ((m & 0xf) == 1 || (m & 0xf) == 2) {
        if (a0 != 5 && a0 != 7 && a0 != 6) {
            ydiv = 2;
        }
    }
    if (a1) {
        a1 = 4;
    } else {
        a1 = 1;
    }
    if (a2) {
        a2 = 2;
    } else {
        a2 = 1;
    }
    if (a0 == 4) {
        s8 *p;

        p = &parameter_up_down_tbl1[(basyo_pack >> 12) * 9];
        for (i = 0; i < 8; i++) {
            if (p[i] > 0) {
                parameter_change(i, p[i] * ((a1 << 12) / a2 / xdiv / ydiv));
            } else {
                parameter_change(i, p[i] << 12);
            }
        }
        if (p[8] < 0) {
            parameter_change(8, p[8] * ((a1 << 12) / a2 / xdiv / ydiv) + (a2 << 16) - 0x10000);
        } else {
            parameter_change(8, (p[8] << 12) + (a2 << 16) - 0x10000);
        }
    } else {
        s8 *p;

        p = &parameter_up_down_tbl0[a0 * 9];
        for (i = 0; i < 8; i++) {
            if (p[i] > 0) {
                parameter_change(i, p[i] * ((a1 << 12) / a2 / xdiv / ydiv));
            } else {
                parameter_change(i, p[i] << 12);
            }
        }
        if (p[8] < 0) {
            parameter_change(8, p[8] * ((a1 << 12) / a2 / xdiv / ydiv) + (a2 << 16) - 0x10000);
        } else {
            parameter_change(8, (p[8] << 12) + (a2 << 16) - 0x10000);
        }
    }
}
