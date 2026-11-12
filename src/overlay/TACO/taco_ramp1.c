/* @SymbolStart: 0x80136D10 */
/* @SymbolSize: 120 */
/* /* @PostProc: rename_register_chain --chain a0:v0 */
/* @PostProc: patch_insn_reg_field 0x40:rt:v0:a0 */
/* taco_ramp1 -- TACO overlay, Address: 0x80136D18 */
#include "types.h"

extern s32 taco_drop_bright;
extern s32 taco_drop_ramp;

extern void sub_sub_step_up(void);
extern void taco_drop(s32, s32);

void taco_ramp1(void)
{
    s32 v0 = taco_drop_ramp;

    s32 v;

    if (v0 < 256) {
        taco_drop_bright = v0 / 2;
    }
    taco_drop(1, 0);
    v = taco_drop_ramp + 4;
    taco_drop_ramp = v;
    if (v >= 257) {
        taco_drop_bright = 128;
        sub_sub_step_up();
    }
}
