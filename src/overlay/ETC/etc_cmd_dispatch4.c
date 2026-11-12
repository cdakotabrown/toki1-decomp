/* @PostProc: swap_registers a0:v1 --unidirectional --exclude-offsets 0x44 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x4c:rt:v1:a0 */
/* Address: 0x801327F8 */
/* Size: 72 bytes (18 instructions) */
#include "types.h"

extern void goto_step_on_down_or_timer();
extern void etc_epilogue_basyo_init();
extern u8 now_sub_sub_step;

void etc_cmd_dispatch4(void)
{
    s32 v1 = now_sub_sub_step;

    switch (v1) {
    case 0:
        etc_epilogue_basyo_init();
        break;
    case 1:
        goto_step_on_down_or_timer(194);
        break;
    }
}
