/* Function: DecDCTReset */
/* Address: 0x800869A0 */
/* Size: 52 bytes (13 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc281 */
/* @CompileFlags: -O2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void ResetCallback(void);
extern void MDEC_reset(s32);

void DecDCTReset(s32 a0) {
    if (!a0) {
        ResetCallback();
    }
    MDEC_reset(a0);
}
