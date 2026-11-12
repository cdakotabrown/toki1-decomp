/* Address: 0x80056B7C */
/* Size: 172 bytes (43 instructions) */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s32 strNext(s32 *arg0);
extern void StFreeRing(s32 arg0);
extern void DecDCTvlc(s32 arg0, s32 arg1);

s32 strNextVlc(s32 *arg0) {
    s32 result;
    s32 counter;

    counter = 0x800000;
    result = strNext(arg0);

    if (result == 0) {
        do {
            counter--;
            if (counter == 0) {
                return -1;
            }
            StFreeRing(result);
            result = strNext(arg0);
        } while (result == 0);
    }

    if (arg0[2] != 0) {
        arg0[2] = 0;
    } else {
        arg0[2] = 1;
    }

    DecDCTvlc(result, arg0[arg0[2]]);

    StFreeRing(result);

    return 0;
}
