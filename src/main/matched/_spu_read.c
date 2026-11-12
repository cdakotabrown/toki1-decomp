/* Address: 0x80095CD8 */
/* Size: 40 bytes (10 instructions) */
/* Region: IDO (Library, >= 0x80086000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 24 --new 32 --shift-ra */
/* @PostProc: swap_instructions 0x8 0xc */
/* Compiler: IDO 5.2 */
/* Likely Symbol: _spu_read (from o.bin, offset +112 bytes from 0x80095D48) */
#include "types.h"

extern void _spu_r(s32, s32);

s32 _spu_read(s32 arg0, volatile s32 arg1) {
    _spu_r(arg0, arg1);
    return arg1;
}
