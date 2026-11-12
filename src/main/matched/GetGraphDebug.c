/* Address: 0x8009FDC0 */
/* Size: 16 bytes (4 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 D_800EBD62;

u8 GetGraphDebug(void) {
    return D_800EBD62;
}
