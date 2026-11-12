/* Address: 0x8007AC78 (true start 0x8007AC70: `lw gSDSerialInfo` is hoisted */
/* Size: 396 bytes (99 instructions) */
/* Region: Game (IDO 5.2) */
/* @SymbolStart: 0x8007AC70 */
/* @SymbolSize: 396 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 gSDInfo;
extern s32 gSDSerialInfo;
extern s16 seq_vol_l;
extern s16 seq_vol_r;
extern s16 cur_seq_num;
extern u16 seq_fade_acc;
extern u16 seq_fade_step;
extern u16 seq_fade_lim;
extern s16 sd_vab_ch0;

extern void seq_start();
extern void seq_restart();
extern void SsSetMVol(s16, s16);
extern void SsSeqSetVol(s16, s16, s16);

void seq_vol_fade()
{
    s16 vol;

    if (gSDSerialInfo & 2) {
        if (seq_busy_check(sd_vab_ch0) != 0) {
            seq_start();
        }
    }
    if (!(gSDInfo & 0x100)) {
        return;
    }
    if (!(gSDSerialInfo & 1)) {
        return;
    }
    seq_fade_acc += seq_fade_lim;
    if (seq_fade_acc & 0xFF00) {
        seq_fade_acc &= 0xFF;
        seq_vol_l -= seq_fade_step;
        if (seq_vol_l < 0) {
            seq_restart();
        } else if ((gSDInfo & 0x100000) && !(gSDInfo & 0x400)) {
            if (seq_vol_r == 0) {
                vol = 0;
            } else {
                vol = seq_vol_l * 127 / seq_vol_r;
            }
            SsSetMVol(vol, vol);
        } else {
            SsSeqSetVol(cur_seq_num, seq_vol_l, seq_vol_l);
        }
    }
}
