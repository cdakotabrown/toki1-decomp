/* @SymbolStart: 0x80148BF4 */
/* @SymbolSize: 144 */
/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: patch_insn_reg_field 0x3c:rd:v0:a3 0x4c:rt:v0:a3 0x50:rs:v0:a3 0x54:rs:v0:a3 */
#include "types.h"

extern s32 tt_str_disp(s16 a0, s16 a1, u8 *buf);

void tt_bin_disp(s16 a0, s16 a1, u32 value, u8 ndigits)
{
    u8 buf[48];
    u8 *p;
    s32 i;
    s32 n;

    p = buf;
    n = --ndigits;
    if (n == 0 || (u32)n >= 32U) {
        n = 31;
    }

    for (i = n; i >= 0; i--) {
        *p = (((value >> i) & 1) ? 1 : 0) + 48;
        p++;
    }
    *p = 0;
    tt_str_disp(a0, a1, buf);
}
