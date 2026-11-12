/* Function: CdDataSync */
/* Address: 0x80087DEC */
/* Size: 32 bytes (8 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @PostProc: strip_leading_instructions --count 19 */
#include "types.h"

extern s32 CD_datasync(void);

s32 CdDataSync(void) {
    return CD_datasync();
}
