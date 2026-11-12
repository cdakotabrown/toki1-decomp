/* Function: CdDataCallback */
/* Address: 0x80087DC8 */
/* Size: 36 bytes (9 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @PostProc: strip_leading_instructions --count 19 */
#include "types.h"

extern s32 DMACallback(s32 arg0, s32 arg1);

s32 CdDataCallback(s32 arg0) {
    return DMACallback(3, arg0);
}
