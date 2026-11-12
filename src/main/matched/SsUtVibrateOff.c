/* Address: 0x8008D798 - 0x8008D7BC (36 bytes) */
/* Region: IDO (>= 0x80081000) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: fix_sll_sra_pattern */
#include "types.h"

extern s32 varargs_spill1(s32 arg0);

s32 SsUtVibrateOff(s16 arg0) {
    return varargs_spill1(arg0);
}
