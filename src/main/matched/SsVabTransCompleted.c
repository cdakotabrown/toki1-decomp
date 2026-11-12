/* Address: 0x8008C6D0 - 0x8008C6F8 (40 bytes) */
/* Region: IDO (>= 0x80081000) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: fix_sll_sra_pattern */
#include "types.h"

extern s32 SpuIsTransferCompleted(s16);

s16 SsVabTransCompleted(s16 arg0) {
    s32 result = SpuIsTransferCompleted(arg0);
    return (s16)result;
}
