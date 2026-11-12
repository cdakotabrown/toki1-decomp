/* Function: SD_CDFrame */
/* Address: 0x80079EDC */
/* Size: 80 bytes (20 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 gSDInfo;
extern s32 gSDSerialInfo;
extern void SD_CDFadeFrame(void);
extern void SD_SerialFrame(void);

void SD_CDFrame(void) {
    s32 flags = gSDInfo;
    if (flags & 0x400) {
        SD_CDFadeFrame();
    }
    if (gSDSerialInfo & 0x200) {
        SD_SerialFrame();
    }
}
