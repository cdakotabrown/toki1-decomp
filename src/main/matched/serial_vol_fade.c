/* Address: 0x8007A59C */
/* Size: 176 bytes (44 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 -cckr */
#include "types.h"

extern s32 gSDInfo;
extern u16 serial_fade_acc;
extern u16 serial_fade_lim;
extern u16 serial_fade_step;
extern s16 serial_vol;
extern void FUN_8007A64C(void);
extern void SsSetSerialVol(s32 a0, s32 a1, s16 a2);

void serial_vol_fade(void) {
    if (gSDInfo & 0x4400) {
        serial_fade_acc += serial_fade_lim;
        if (serial_fade_acc & 0xFF00) {
            serial_fade_acc &= 0xFF;
            serial_vol -= serial_fade_step;
            if (serial_vol <= 0) {
                FUN_8007A64C();
            } else {
                SsSetSerialVol(0, serial_vol, serial_vol);
            }
        }
    }
}
