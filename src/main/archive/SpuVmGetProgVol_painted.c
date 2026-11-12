/* Address: 0x8008EA64 */
/* Size: 88 bytes (22 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: custom_script postproc_8008EA64.py */
#include "types.h"

extern u8 * volatile _svm_pg;
s32 SpuVmVSetUp(s16, s16);

s32 SpuVmGetProgVol(s16 arg0, s16 arg1) {
    if (SpuVmVSetUp(arg0, arg1) == 0) {
        return _svm_pg[arg1 * 16 + 1];
    }
    return -1;
}
