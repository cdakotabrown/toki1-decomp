/* Likely Symbol: set_tarao_rect (from o.bin, offset +96 bytes from 0x8008BE5C) */
/* Address: 0x8008BEBC */
/* Size: 32 bytes (8 instructions) */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80081000-0x800BFFFF) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern void SpuQuit(void);

void SsQuit(void) {
    SpuQuit();
}
