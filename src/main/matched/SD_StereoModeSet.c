/* Address: 0x8007BD8C */
/* Size: 236 bytes (59 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=17:t1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void SsSetMono();
extern void SsSetStereo();
extern void CdMix();

extern s32 gSDInfo;
extern s16 seq_vol_l;
extern s16 seq_vol_r;
extern s16 voice_vol_l;
extern s16 voice_vol_r;

void SD_StereoModeSet(mode)
    u16 mode;
{
    u8 buf[4];
    u8 d;

    if (mode == 1) {
        SsSetMono();
        gSDInfo &= ~0x10;
        gSDInfo |= 0x20;
        buf[2] = 79; buf[0] = 79;
        buf[3] = 79; buf[1] = 79;
        d = 24;
    } else {
        SsSetStereo();
        gSDInfo &= ~0x20;
        gSDInfo |= 0x10;
        buf[2] = 127; buf[0] = 127;
        buf[3] = 0;
        buf[1] = 0;
        d = 0;
    }
    CdMix(buf);
    seq_vol_l = seq_vol_r = 80 - d;
    voice_vol_l = voice_vol_r = 90 - d;
}
