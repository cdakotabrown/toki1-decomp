/* Debug Symbol: CdReset (from o.bin, high match) */
/* Address: 0x800878A0 - 0x800878C0 (32 bytes) */
/* Region: Library (>= 0x80086000) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --fill-earliest */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 CD_init(void);

s32 CdReset(void) {
    return CD_init() == 0;
}
