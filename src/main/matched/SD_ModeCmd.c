/* Function: SD_ModeCmd */
/* Address: 0x8007A4CC (true start; the old symbol 0x8007A4D8 was the */
/* Size: 160 bytes (40 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @SymbolStart: 0x8007A4CC */
/* @SymbolSize: 160 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void SD_StereoModeSet(u16);
extern void SD_StateClear(void);
extern void SD_SerialReset(void);
extern void SD_ReverbReset(void);
extern s32 gSDInfo;

void SD_ModeCmd(u16 mode) {
    mode &= 0xFF;
    switch (mode) {
        case 1:
        case 2:
            SD_StereoModeSet(mode);
            break;
        case 3:
            SD_StateClear();
            break;
        case 6:
            SD_SerialReset();
            break;
        case 4:
            SD_ReverbReset();
            break;
        case 5:
            gSDInfo |= 0x100000;
            break;
    }
}
