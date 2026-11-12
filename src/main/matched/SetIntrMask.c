/* Address: 0x800B3BDC, Size: 24 bytes (6 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_registers t6:v1 --unidirectional */
#include "types.h"

extern u16 *_mask;

u16 SetIntrMask(s32 a) {
    u16 *p;
    u16 old;

    p = _mask;
    old = *p;
    *p = (u16)a;
    return old;
}
