/* Address: 0x800B581C */
/* Size: 56 bytes (14 instructions) */
/* Region: Library (Psy-Q GCC) */
/* Compiler: gcc260 */
/* @Compiler: gcc260 */
/* @PostProc: strip_leading_instructions --count 19 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x0:rt:a0:v1 0x4:rs:a0:v1 0xc:rt:a0:v1 0xc:rd:v0:v1 0x10:rt:v1:v0 0x14:rs:v1:v0 0x14:rt:v1:v0 0x1c:rs:v0:v1 0x1c:rt:v1:v0 0x1c:rd:v0:v1 0x20:rs:v0:v1 */
#include "types.h"

extern s16 *libapi_rcnt_regs_ptr;

u16 GetRCnt(s32 a0) {
    s32 idx;
    u16 ret;

    idx = a0 & 0xffff;
    if (idx >= 3) {
        return 0;
    }
    ret = (u16)libapi_rcnt_regs_ptr[idx * 8];
    return ret;
}
