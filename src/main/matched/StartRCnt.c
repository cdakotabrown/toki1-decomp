/* Address: 0x800B5854 */
/* Size: 52 bytes (13 instructions) */
/* Region: Library (Psy-Q GCC) */
/* Compiler: gcc260 */
/* @Compiler: gcc281 */
/* @CompileFlags: -O2 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_registers --from a0 --to v1 --bidirectional --exclude-offsets 0x0 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x18 */
/* @PostProc: patch_insn_reg_field --patch 0x10:rt:a0:a1 --patch 0x14:rs:a0:a1 */
#include "types.h"

extern s32 libapi_rcnt_irq_bit_tbl[];
extern s32 *libapi_rcnt_irq_regs_ptr;

s32 StartRCnt(s32 a0) {
    s32 idx;
    s32 mask;

    idx = a0 & 0xffff;
    mask = libapi_rcnt_irq_bit_tbl[idx];
    libapi_rcnt_irq_regs_ptr[1] = libapi_rcnt_irq_regs_ptr[1] | mask;
    return idx < 3;
}
