/* Likely Symbol: biorhythm_sf2 (from o.bin, offset +112 bytes from 0x8006C418) */
/* Address: 0x8006C3A8 */
/* Size: 44 bytes */
/* Region: GCC (0x80041000-0x80080FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void biorhythm_sf2(s32 a, s32 b, s32 c, s32 d);

void biorhythm_sf(void) {
    biorhythm_sf2(1, 1, 1, 2);
}
