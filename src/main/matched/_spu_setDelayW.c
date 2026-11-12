/* Address: 0x80095A64, Size: 40 bytes (10 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_registers t6:v1 --unidirectional */
/* @PostProc: swap_registers t7:v0 --unidirectional */
/* @PostProc: swap_registers t8:v0 --unidirectional */
/* @PostProc: swap_registers t9:v0 --unidirectional */
#include "types.h"

extern u32 *libspu_dev_delay_reg_ptr;

void _spu_setDelayW(void) {
    u32 *p;

    p = libspu_dev_delay_reg_ptr;
    *p = (*p & 0xF0FFFFFFu) | 0x20000000u;
}
