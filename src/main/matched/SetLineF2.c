/* Address: 0x800A2F90 */
/* Size: 40 bytes (10 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -- libgpu, recompiled */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions 0x1c 0x24 */
#include "types.h"

void SetLineF2(u32 *p) {
    *p = (*p & 0x00FFFFFF) | (3 << 24);
    ((u8 *)p)[7] = 0x40;
}
