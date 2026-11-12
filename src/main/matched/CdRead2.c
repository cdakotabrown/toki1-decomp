/* Function: CdRead2 */
/* Address: 0x80087EA8 */
/* Size: 548 bytes (137 instructions) */
/* Region: Library (0x80086000+), Psy-Q GCC 2.6.0 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --fill-epilogue */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0xf0 --offset2 0xfc */
/* @PostProc: swap_instructions --offset1 0xf4 --offset2 0x104 */
/* @PostProc: swap_instructions --offset1 0xf8 --offset2 0x108 */
/* @PostProc: swap_instructions --offset1 0xfc --offset2 0x10c */
/* @PostProc: swap_instructions --offset1 0x104 --offset2 0x110 */
/* @PostProc: swap_instructions --offset1 0x108 --offset2 0x10c */
/* @PostProc: swap_instructions --offset1 0x10c --offset2 0x110 */
/* @PostProc: swap_instructions --offset1 0x1f8 --offset2 0x1fc */
/* @PostProc: swap_instructions --offset1 0x1fc --offset2 0x200 */
#include "types.h"

extern s32 CD_cw(s32 a0, s32 a1, s32 a2, s32 a3);
extern void DMACallback(s32 a0, s32 a1);
extern s32 CD_cbsync;
extern s32 CD_status;
extern s32 libcd_setmode_needs_setloc;
extern s32 CD_cbready;
extern s32 D_8008B140;
extern s32 D_80088370;

s32 CdRead2(s32 arg0) {
    s32 saved;
    s32 saved2;
    u8 buf[8];
    u8 *ptr;
    s32 retry;
    s32 limit;
    s32 limit2;
    s32 limit3;
    s32 flag;

    saved = CD_cbsync;
    buf[0] = (u8)arg0;
    limit = -1;
    retry = 3;
    ptr = buf;

loop_top1:
    CD_cbsync = 0;
    if (CD_status & 0x10) {
        CD_cw(1, 0, 0, 0);
        goto decrement1;
    }

    if (ptr != 0) {
        if (libcd_setmode_needs_setloc != 0) {
            if (CD_cw(2, (s32)ptr, 0, 0) != 0) {
                goto decrement1;
            }
        }
    }

    CD_cbsync = saved;
    if (CD_cw(14, (s32)ptr, 0, 0) == 0) {
        goto check_flag;
    }

decrement1:
    retry--;
    if (retry != limit) {
        goto loop_top1;
    }
    CD_cbsync = saved;

check_flag:
    if (!(arg0 & 0x100)) {
        goto second_path;
    }

    DMACallback(3, (s32)&D_8008B140);

    saved2 = CD_cbsync;
    CD_cbready = (s32)&D_80088370;
    retry = 3;
    flag = 0;
    limit2 = -1;

loop_top2:
    CD_cbsync = 0;

    if (CD_status & 0x10) {
        goto flag_handler2;
    }

    CD_cbsync = saved2;
    if (CD_cw(27, 0, 0, 0) != 0) {
        goto dec2;
    }
    goto done;

flag_handler2:
    CD_cw(1, 0, 0, 0);
dec2:
    retry--;
    if (retry == limit2) {
        goto fail;
    }
    goto loop_top2;

second_path:
    retry = 3;
    saved2 = CD_cbsync;
    flag = 0;
    limit3 = -1;

loop_top3:
    CD_cbsync = 0;

    if (CD_status & 0x10) {
        goto flag_handler3;
    }

    CD_cbsync = saved2;
    if (CD_cw(27, 0, 0, 0) != 0) {
        goto dec3;
    }
    goto done;

flag_handler3:
    CD_cw(1, 0, 0, 0);
dec3:
    retry--;
    if (retry != limit3) {
        goto loop_top3;
    }

fail:
    CD_cbsync = saved2;
    flag = -1;

done:
    return flag + 1;
}
