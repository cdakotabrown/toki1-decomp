/* Likely Symbol: startIntrVSync (from o.bin, offset +108 bytes from 0x800B42EC) */
/* Address: 0x800B4280 - 0x800B42A0 (32 bytes) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @PostProc: strip_leading_instructions --count 19 */
/* @PostProc: maspsx --aspsx-version=2.67 */
/* @PostProc: fill_gcc_delay_slots */
#include "types.h"

extern void StopPAD(void);

void PadStop(void) {
    StopPAD();
}
