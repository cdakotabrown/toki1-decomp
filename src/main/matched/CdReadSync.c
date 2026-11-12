/* Function: CdReadSync */
/* Address: 0x80087E0C */
/* Size: 32 bytes (8 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @PostProc: strip_leading_instructions --count 19 */
#include "types.h"

extern void CD_readsync(void);

void CdReadSync(void) {
    CD_readsync();
}
