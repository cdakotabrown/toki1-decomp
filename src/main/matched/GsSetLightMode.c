/* Function: GsSetLightMode */
/* Address: 0x8009D1A0 */
/* Size: 128 bytes (32 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @PostProc: strip_leading_instructions --count 19 */
/* @PostProc: maspsx --aspsx-version=2.67 */
/* @PostProc: fill_gcc_delay_slots */
#include "types.h"

extern s32 GsLIGHT_MODE;
extern void printf(char *, s32);
extern char libgs_bad_light_mode_fmt[];

void GsSetLightMode(s32 arg0) {
    switch (arg0) {
        case 0:
            GsLIGHT_MODE = 0;
            break;
        case 1:
            GsLIGHT_MODE = arg0;
            break;
        case 2:
            GsLIGHT_MODE = arg0;
            break;
        case 3:
            GsLIGHT_MODE = arg0;
            break;
        default:
            printf(libgs_bad_light_mode_fmt, arg0);
            return;
    }
}
