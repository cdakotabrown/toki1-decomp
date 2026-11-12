/* @SymbolStart: 0x801446C4 */
/* @SymbolSize: 72 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x20:rs:v0:v1 0x20:rt:at:v0 0x24:rs:v0:v1 0x2c:rs:at:v0 */
/* Address: 0x801446CC (cross-prologue head at 0x801446C4 absorbed) */
/* Size: 72 bytes (18 instructions) */
#include "types.h"

extern void card_state_clear();
extern void goto_step();
extern u8 screen_bri;

void save_file_fade_out(void)
{
    s32 v1 = screen_bri;

    screen_bri = (u8)(v1 - 1);

    if ((u32)v1 >= 8) {
        return;
    }

    screen_bri = 0;
    card_state_clear();
    goto_step(17);
}
