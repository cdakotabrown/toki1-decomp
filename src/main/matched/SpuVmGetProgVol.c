/* Address: 0x8008EA64 */
/* Size: 88 bytes (22 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=0 IDO52_UGEN_TEMP_REG=3:t7,4:t8,5:t6,6:t9 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 24 --new 32 */
/* @PostProc: remap_stack_slots --map '20:28,28:36' */
/* @PostProc: patch_insn_reg_field 0x10:rd:t6:a0 0x18:rt:t6:a0 0x34:rs:t8:t6 0x34:rt:t6:t8 */








































#include "types.h"

extern u8 * volatile _svm_pg;

extern s32 SpuVmVSetUp(s16 arg0, s16 arg1);

s32 SpuVmGetProgVol(s16 arg0, s16 arg1)
{
#line 5
    if (SpuVmVSetUp(arg0, arg1) == 0) {
#line 5
        return _svm_pg[arg1 * 16 + 1];
#line 5
    }
#line 5
    return -1;
}







