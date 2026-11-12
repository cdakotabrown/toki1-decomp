/* Function: CdSync */
/* Address: 0x800878C0 */
/* Size: 32 bytes (8 instructions) */
/* Region: Library (0x80086000+) */
/* Likely Symbol: CdReady (from o.bin, offset +28 bytes from 0x800878DC) */
/* @Compiler: gcc260 */
/* @PostProc: strip_leading_instructions --count 19 */
#include "types.h"

extern void CD_sync(void);

void CdSync(void) {
    CD_sync();
}
