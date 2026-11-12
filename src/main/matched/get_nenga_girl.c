/* Function: get_nenga_girl */
/* Address: 0x800610D8 */
/* Size: 420 bytes (105 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_frame_size_only --old 56 --new 72 */
/* @PostProc: remap_stack_slots --map 32:44,36:52,37:53,38:54,39:55 */
/* @PostProc: swap_registers_liveness --target-exe disks/SLPS_000.65 --address 0x800610D8 */
/* @PostProc: patch_insn_reg_field --patch 0x3c:rd:t8:t9 --patch 0x40:rs:t8:t9 --patch 0x58:rt:t0:t1 --patch 0x5c:rt:t0:t1 --patch 0x5c:rs:t0:t1 --patch 0x64:rt:t0:t1 */
#include "types.h"

extern s32 girl0_flag;
extern s32 girl1_flag;
extern s32 girl2_flag;
extern u8 screen_w;

extern void get_h_yuukou_table(u8 *);

s32 get_nenga_girl(void) {
    u8 buffer[20];
    s32 count;
    s32 i;

    count = 0;
    get_h_yuukou_table(buffer);

    if (buffer[0] >= (u32)77) {
        if ((girl0_flag << 30) < 0) {
            count = 1;
        }
    }
    if (buffer[1] >= (u32)77) {
        if ((girl1_flag << 30) < 0) {
            count++;
        }
    }
    if (buffer[2] >= (u32)77) {
        if ((girl2_flag << 30) < 0) {
            count++;
        }
    }

    for (i = 3; i < 11; i++) {
        if (buffer[i] >= (u32)77) {
            if ((*(s32 *)(&screen_w + i * 56 + 456) << 30) < 0) {
                count++;
            }
        }
    }

    return count;
}
