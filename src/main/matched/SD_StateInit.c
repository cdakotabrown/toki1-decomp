/* Address: 0x8007BB3C */
/* Size: 316 bytes (79 instructions) */
/* Region: Game */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"
#include "tokimeki/sd.h"

extern u32 gSDInfo;
extern u32 gSDSerialInfo;
extern s16 serial_vol;
extern s16 serial_vol_base;
extern s16 seq_vol_l;
extern s16 seq_vol_r;
extern s16 voice_vol_l;
extern s16 voice_vol_r;
extern s16 cur_seq_num;
extern s16 gSDSeqNo;
extern s16 seq_req;
extern s16 seq_fade_acc;
extern s16 seq_fade_step;
extern s16 seq_fade_lim;
extern s16 serial_fade_acc;
extern s16 serial_fade_step;
extern s16 serial_fade_lim;
extern s16 sd_vab_ch0;
extern s16 sd_vab_ch1;
extern s16 sd_vab_ch2;
extern s16 sd_vab_ch3;

void SD_StateInit(void) {
    gSDSerialInfo = 0;
    gSDInfo = 16;
    (*(u32 *)&(*(SD_INFO *)&gSDInfo).xa_start) = 0;
    (*(u32 *)&(*(SD_INFO *)&gSDInfo).xa_end) = 0;
    (*(u32 *)&(*(SD_INFO *)&gSDInfo).xa_next_start) = 0;
    (*(u32 *)&(*(SD_INFO *)&gSDInfo).xa_next_end) = 0;
    (*(u32 *)&(*(SD_INFO *)&gSDInfo).xa_resume) = 0;
    serial_vol = 100;
    serial_vol_base = 100;
    seq_vol_l = 80;
    seq_vol_r = 80;
    voice_vol_l = 90;
    voice_vol_r = 90;
    cur_seq_num = 0;
    gSDSeqNo = -1;
    seq_req = -1;
    seq_fade_acc = 0;
    seq_fade_step = 128;
    seq_fade_lim = 255;
    serial_fade_acc = 0;
    serial_fade_step = 128;
    serial_fade_lim = 255;
    sd_vab_ch2 = 0;
    sd_vab_ch0 = 1;
    sd_vab_ch1 = 2;
    sd_vab_ch3 = 3;
    (*(s16 *)&(*(SD_INFO *)&gSDInfo).seq_bank) = 0;
    (*(SD_INFO *)&gSDInfo).xa_chan = 0;
    (*(SD_INFO *)&gSDInfo).xa_chan_next = 0;
    (*(SD_INFO *)&gSDInfo).xa_file = 0;
    (*(SD_INFO *)&gSDInfo).xa_file_next = 0;
}
