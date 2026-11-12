/* Address: 0x80067F2C */
/* Size: 36 bytes of code (9 instructions); the recorded 52-byte symbol also */
/* Region: Game (IDO 5.2 -O2, -Wc,-pic1) */
/* @SymbolSize: 40 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_AS1_KEEP_ANDI=occ:4,6 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_insn_reg_field 0x0:rt:v1:v0 0x4:rs:v1:v0 0x14:rt:v1:t7 0x18:rs:v1:t7 0x18:rt:t7:v0 0x1c:rt:t7:v0 */
#include "types.h"

extern u8 cal_cw_mode;

void bustup_flag_on(void)
{
    u8 v;

    v = cal_cw_mode;
    if (!(v & 1)) {
        cal_cw_mode = v | 3;
    }
}
