/* Likely Symbol: GsDrawOt (from o.bin, offset +120 bytes from 0x8009CC08) */
/* Address: 0x8009CB90 - 0x8009CBB0 (32 bytes) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @PostProc: strip_leading_instructions --count 19 */
/* @PostProc: maspsx --aspsx-version=2.67 */
/* @PostProc: fill_gcc_delay_slots */
#include "types.h"

extern void SetGeomScreen(void);

void GsSetProjection(void) {
    SetGeomScreen();
}
