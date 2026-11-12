/* Likely Symbol: set_tarao_rect (from o.bin, offset +112 bytes from 0x8008BE5C) */
/* Address: 0x8008BDEC - 0x8008BE0C (32 bytes) */
/* Region: IDO (>= 0x80081000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern void _SsStart(s32 arg0);

void SsStart2(void) {
    _SsStart(0);
}
