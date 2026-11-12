/* Address: 0x800A307C */
/* Size: 52 bytes (13 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -- libgpu, recompiled */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions 0x1c 0x20 */
/* @PostProc: swap_instructions 0x18 0x1c */
/* @PostProc: swap_instructions 0x24 0x28 */
#include "types.h"

void SetLineG4(u32 *p) {
    *p = (*p & 0x00FFFFFF) | (9 << 24);
    ((u8 *)p)[7] = 0x5C;
    p[9] = 0x55555555;
}
