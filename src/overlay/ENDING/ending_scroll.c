/* @SymbolStart: 0x80136C00 */
/* @SymbolSize: 124 */
/* @PostProc: patch_insn_reg_field 0x10:rt:a0:v0 0x14:rs:a0:v0 0x14:rt:a0:v0 0x1c:rs:a0:v0 0x30:rt:a0:v0 0x34:rs:a0:v0 0x34:rt:a0:v0 0x3c:rs:a0:v0 0x3c:rt:v1:t7 0x40:rs:v1:t7 0x44:rs:a0:v0 0x50:rt:a0:v0 0x54:rs:a0:v0 0x54:rt:a0:v0 0x5c:rs:a0:v0 */
/* ending_scroll - ENDING overlay, Address: 0x80136C00, Size: 124 bytes (31 instructions) */
#include "types.h"

extern s32 step_counter;
extern void ending_grid_scroll();
extern void ending_item_scroll();
extern void sub_sub_step_up();

void ending_scroll(void)
{
    s32 v0 = step_counter;
    s32 t7;

    if ((v0 & 3) == 0) {
        ending_grid_scroll(1);
        v0 = step_counter;
    }
    t7 = v0 & 0x0F;
    v0 += 1;
    if (t7 == 0) {
        ending_item_scroll(1);
        v0 = step_counter;
        v0 += 1;
    }
    step_counter = v0;
    if ((u32)v0 < 1088) {
        return;
    }
    sub_sub_step_up();
}
