/* Address: 0x800B5888 */
/* Size: 52 bytes (13 instructions) */
/* Region: Library (Psy-Q GCC) */
/* Compiler: gcc260 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 libapi_rcnt_irq_bit_tbl[];
extern s32 *libapi_rcnt_irq_regs_ptr;

s32 StopRCnt(s32 a0) {
    s32 idx;

    idx = a0 & 0xffff;
    libapi_rcnt_irq_regs_ptr[1] = libapi_rcnt_irq_regs_ptr[1] & ~libapi_rcnt_irq_bit_tbl[idx];
    return 1;
}
