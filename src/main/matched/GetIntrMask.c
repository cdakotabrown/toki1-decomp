/* Address: 0x800B3BC4 */
/* Size: 24 bytes (6 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u16 *_mask;

u32 GetIntrMask(void) {
    return *_mask;
}
