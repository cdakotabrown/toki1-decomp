/* Function: CdReady */
/* Address: 0x800878E0 */
/* Size: 32 bytes (8 instructions) */
/* Region: Library (0x80086000+) */
/* Debug Symbol: CdReady (from o.bin, high match) */
/* @Compiler: gcc260 */
/* @PostProc: strip_leading_instructions --count 19 */
#include "types.h"

extern void CD_ready(void);

void CdReady(void) {
    CD_ready();
}
