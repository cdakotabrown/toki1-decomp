/* Address: 0x800869D4 */
/* Size: 12 bytes (3 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

u32 DecDCTBufSize(u16 *p) {
    return *p;
}
