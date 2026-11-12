/* Address: 0x80097668 - 0x80097688 (32 bytes) */
/* Region: IDO (>= 0x80081000) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern void _SpuInit(s32 arg0);

void SpuInitHot(void) {
    _SpuInit(1);
}
