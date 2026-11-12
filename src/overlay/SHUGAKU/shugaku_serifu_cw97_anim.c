/* @SymbolSize: 76 */
/* /* @PostProc: patch_insn_reg_field 0x30:rt:t6:v0 0x38:rs:t6:v0 0x40:rt:t7:t6 0x48:rt:t7:t6 */
/* Address: 0x80134014 */
/* Size: 76 bytes (19 instructions) */
#include "types.h"

extern s32 step_counter;
extern s16 cw97_anim;

extern void func_801343A0(void);

void shugaku_serifu_cw97_anim(void)
{
    s32 v1;
    u32 cmp;

    func_801343A0();
    v1 = step_counter;
    cmp = ((u32)(v1 ^ 0x3C) < 1U);
    v1++;
    step_counter = v1;
    if (cmp != 0) {
        cw97_anim = 1;
    }
}
