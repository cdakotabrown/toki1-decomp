/* Function: SD_CDLevelGet */
/* Address: 0x8007C5AC */
/* Size: 68 bytes (17 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 gSDInfo;
extern u16 gSDCDLevelInfo;
extern u16 D_8012DED2;

s32 SD_CDLevelGet(void) {
    s16 r;

    if (gSDInfo & 0x400) {
        r = (s16)((u32)(gSDCDLevelInfo + D_8012DED2) >> 1);
        return r;
    }
    return 0;
}
