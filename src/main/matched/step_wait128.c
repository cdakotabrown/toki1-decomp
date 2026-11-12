/* Function: step_wait128 */
/* Address: 0x80085B28  (registered address corrected 2026-09-17; see below) */
/* Size: 72 bytes (18 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @SymbolStart: 0x80085B28 */
/* @SymbolSize: 72 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_insn_reg_field 0x10:rt:t6:v0 0x20:rs:t6:v0 */
#include "types.h"

extern s32 step_counter;
extern void k_reset(s32);
extern void sub_sub_step_up(void);

void step_wait128(void) {
    s32 mode = step_counter;
    s32 cond = ((u32)mode < 129) ^ 1;
    step_counter = mode + 1;
    if (cond) {
        k_reset(1);
        sub_sub_step_up();
    }
}
