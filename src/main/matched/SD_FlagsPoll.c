/* Address: 0x8007A0A0 */
/* Size: 112 bytes (28 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x8007A098 */
/* @SymbolSize: 120 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 gSDSerialInfo;

extern void SD_VabLoadNext(void);
extern void seq_vol_fade(void);
extern void SD_FadePoll(void);

void SD_FlagsPoll(void) {
    u32 param;

    param = (u32)gSDSerialInfo;
    if (param == 0) {
        return;
    }
    if (param >> 24) {
        SD_VabLoadNext();
        param = (u32)gSDSerialInfo;
    }
    if (param & 0xFF) {
        seq_vol_fade();
        param = (u32)gSDSerialInfo;
    }
    if (param & 0xFF00) {
        SD_FadePoll();
    }
}
