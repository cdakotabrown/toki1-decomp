/* Address: 0x800531C4 */
/* Size: 48 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 now_month;
extern u8 now_day;
extern s32 birth_day_check_days(s32 a0, s32 a1, s32 a2);

s32 birth_day_check(s32 a0) {
    return birth_day_check_days(a0, now_month, now_day);
}
