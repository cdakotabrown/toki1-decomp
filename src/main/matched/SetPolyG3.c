/* Address: 0x800A2D60 */
/* Size: 40 bytes (10 instructions) */
/* Region: IDO (Library, >= 0x80086000) */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_instructions 0x1c 0x24 */
#include "types.h"

void SetPolyG3(u32 *p) {
    *p = (*p & 0x00FFFFFF) | 0x06000000;
    ((u8 *)p)[7] = 48;
}
