/* Address: 0x800576F8 */
/* Size: 12 bytes (3 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 fade_bri;

void set_dec_bri(s32 arg0) {
    fade_bri = (u8)arg0;
}
