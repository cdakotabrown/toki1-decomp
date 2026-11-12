/* Address: 0x80134B6C, Size: 148 bytes (37 instructions) */
/* @PostProc: patch_stack_complete --old 72 --new 96 */
/* @PostProc: remap_stack_slots --from 56 --to 68 --from 60 --to 72 --from 64 --to 76 --from 68 --to 80 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x4c */
/* @PostProc: patch_insn_reg_field 0x20:rt:a1:v1 0x2c:rt:v1:a1 0x3c:rt:v1:a1 0x40:rs:v1:a1 0x40:rt:v1:a1 0x44:rs:a1:v1 0x44:rt:a1:v1 0x50:rs:a1:v1 */
#include "types.h"

extern void menu_set();
extern void sub_sub_step_up();
extern void option_menu_box_show();

void option_submenu_init(void)
{
    s16 buf2[2];
    s16 buf0[2];
    s16 buf1[2];
    s16 buf3[2];
    s32 i;
    s32 val;

    val = -40;
    for (i = 0; i < 2; i++) {
        buf0[i] = val;
        val += 16;
        buf2[i] = 0;
        buf1[i] = 108;
        buf3[i] = 16;
    }

    menu_set(1, 2, buf2, buf0, buf1, buf3);
    sub_sub_step_up();
    option_menu_box_show();
}
