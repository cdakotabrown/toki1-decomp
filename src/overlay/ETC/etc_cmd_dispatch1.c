/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 */
/* Address: 0x801325EC */
/* Size: 72 bytes (18 instructions) */
#include "types.h"

extern void pad_wait_sub_step();
extern void etc_bustup_scroll_basyo_init();
extern u8 now_sub_sub_step;

void etc_cmd_dispatch1(void)
{
    s32 v1 = now_sub_sub_step;

    switch (v1) {
    case 0:
        etc_bustup_scroll_basyo_init();
        break;
    case 1:
        pad_wait_sub_step();
        break;
    }
}
