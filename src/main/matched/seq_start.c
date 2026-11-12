/* Function: seq_start */
/* Address: 0x8007AF9C */
/* Size: 568 bytes (142 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileEnv: IDO52_FORM_SITE=const:0xfffffffd:78:1 IDO52_FORCE_WEB_COLOR=30:a3 IDO52_UGEN_DEST_TEMP=53 IDO52_UGEN_TEMP_REG=30:t4 IDO52_AS1_TIE_TAKE=258,267,270 IDO52_AS1_TIE_REJECT=271,276 */
#include "types.h"

extern s32 tt_step_check(void);
extern s32 ending_step_check(void);
extern s32 scene_date_check(void);
extern void SD_ReverbReset(void);
extern void seq_restart(void);
extern void SsUtSetReverbDepth(s32 a0, s16 a1);
extern s16 SsSeqOpen(u32 a0, s16 a1);
extern void device_port_error_set(u32 a0);
extern void SsUtGetVabHdr(s16 a0, void *a1, s32 a2, s16 a3);
extern void SsSetMVol(s32 a0, s32 a1);
extern void SsSeqSetVol(s16 a0, s16 a1, s16 a2, s32 a3);
extern void SsSeqPlay(s16 a0, s32 a1, s32 a2);

extern s32 gSDInfo;
extern s32 gSDSerialInfo;
extern u16 gSDSeqBank;
extern u16 sd_vab_ch0;
extern s16 cur_seq_num;
extern s16 gSDSeqNo;
extern s16 seq_vol_l;
extern s16 seq_vol_r;
extern s16 gSDSeqReverb[16][16];
extern u32 gSDSeqAdrs[16][16];

void seq_start(void) {
    s32 padh;
    u32 buf[8];
    s32 padl[2];
    s16 temp;
    s32 x;

    if (tt_step_check() == 0) {
        if (ending_step_check() == 0) {
            if (sd_vab_ch0 != 2) {
                SD_ReverbReset();
            }
        }
    }

    if ((gSDInfo << 3) < 0) {
        if (scene_date_check() != 0) {
            SsUtSetReverbDepth(0, 0);
        }
    } else {
        SsUtSetReverbDepth(gSDSeqReverb[gSDSeqBank][gSDSeqNo], gSDSeqReverb[gSDSeqBank][gSDSeqNo]);
    }

    temp = SsSeqOpen(gSDSeqAdrs[gSDSeqBank][gSDSeqNo], (s16)sd_vab_ch0);
    cur_seq_num = temp;
    if (temp == -1) {
        device_port_error_set(((sd_vab_ch0 << 8) & 0xFF00) | 0x01040000 | (gSDSeqBank & 0xFF));
        seq_restart();
        return;
    }

    gSDInfo |= 0x100;
    gSDSerialInfo &= ~2;
    gSDInfo &= ~0x1000;
    gSDSerialInfo &= ~1;
    gSDInfo &= ~0x100000;
    gSDSerialInfo &= ~2;
    seq_vol_l = 0;
    seq_vol_l |= seq_vol_r;
    SsUtGetVabHdr((s16)sd_vab_ch0, buf, -3, seq_vol_l);

    temp = ((u8 *)buf)[24];
    temp -= 25;
    if (gSDInfo & 0x20) {
        temp -= 24;
    }
    seq_vol_r = temp;
    seq_vol_l = temp;

    SsSetMVol(0x7F, 0x7F);
    x = seq_vol_l;
    SsSeqSetVol(cur_seq_num, (s16)x, (s16)(u16)x, x);
    SsSeqPlay(cur_seq_num, 1, 1);
}
