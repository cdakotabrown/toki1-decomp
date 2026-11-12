/* Address: 0x800A2C78 */
/* Size: 32 bytes (8 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 -- libgpu, recompiled (not Sony's object) */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

void TermPrim(u32 *p) {
    *p = (*p & 0xFF000000) | 0x00FFFFFF;
}
