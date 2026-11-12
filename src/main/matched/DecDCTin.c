/* Address: 0x80086A20 */
/* Size: 128 bytes (32 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: Psy-Q GCC 2.6.0 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --delay-slot-load-nop --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void MDEC_in(u32 *arg0, u16 arg1);

void DecDCTin(u32 *arg0, u32 arg1) {
    if (arg1 & 1) {
        *arg0 = *arg0 & 0xF7FFFFFF;
    } else {
        *arg0 = *arg0 | 0x08000000;
    }

    if (arg1 & 2) {
        *arg0 = *arg0 | 0x02000000;
    } else {
        *arg0 = *arg0 & 0xFDFFFFFF;
    }

    MDEC_in(arg0, *(u16 *)arg0);
}
