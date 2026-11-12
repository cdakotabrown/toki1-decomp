/* Address: 0x80095754, Size: 88 bytes (22 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @CompileFlags: -O1 */
/* @PostProc: strip_leading_instructions --count 26 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map '30:24' */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x2C */
/* @PostProc: swap_registers t6:v0 --unidirectional */
/* @PostProc: swap_registers t7:v1 --unidirectional */
/* @PostProc: swap_registers t8:t6 --unidirectional */
/* @PostProc: swap_registers t9:t7 --unidirectional */
/* @PostProc: swap_registers t0:t8 --unidirectional */
/* @PostProc: swap_registers t1:v1 --unidirectional */
/* @PostProc: swap_registers t2:v1 --unidirectional */
/* @PostProc: swap_registers t3:v1 --unidirectional */
/* @PostProc: swap_registers t4:t9 --unidirectional */
#include "types.h"

extern void _waitfs(void);
extern u32 _spu_RXX;

s32 _spu_reset(void) {
    u16 val;

    val = ((u16 *)_spu_RXX)[213];
    ((u16 *)_spu_RXX)[213] = ((val & 0x7FFF) & 0xFFFF) & 0xFFCF;
    _waitfs();
    ((u16 *)_spu_RXX)[213] = (val & 0xFFCF) & 0xFFFF;
    return 0;
}
