/* Address: 0x80097648 - 0x80097668 (32 bytes) */
/* Region: IDO (>= 0x80081000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
#include "types.h"

extern void _SpuInit(s32 arg0);

void SpuInit(void) {
    _SpuInit(0);
}
