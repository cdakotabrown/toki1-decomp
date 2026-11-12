/* Address: 0x800A2C58 */
/* Size: 32 bytes (8 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -- libgpu, recompiled (not Sony's object) */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

void CatPrim(u32 *p0, u32 *p1) {
    u32 y = *p0;

    *p0 = ((((u32)p1 ^ y) << 8) >> 8) ^ y;
}
