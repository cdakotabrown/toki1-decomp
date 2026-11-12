/* Address: 0x8008B550 */
/* Size: 32 bytes (8 instructions) */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80081000-0x800BFFFF) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern void _SsInit(s32);

void SsInitHot(void) {
    _SsInit(1);
}
