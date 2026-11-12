/* Function: seq_ctrl */
/* Address: 0x8007B1D4 */
/* Size: 368 bytes (92 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: custom_script custom_func_8007B1D4.py */
#include "types.h"

extern volatile s32 gSDInfo;
extern s32 gSDSerialInfo;
extern s16 cur_seq_num;
extern s16 gSDSeqNo;
extern s16 seq_req;

extern void SsSeqSetVol(s16, s16, s16);
extern void SsSeqStop(s16);
extern void SsSeqClose(s16);
extern void SsSeqPause(s16);
extern void SsSeqReplay(s16);

void seq_ctrl(s32 arg0) {
    s32 temp;

    switch (arg0 & 0xFF) {
    case 0x71:
        temp = gSDInfo;
        if ((temp & 0x100) || (temp & 0x1000)) {
            SsSeqSetVol(cur_seq_num, 0, 0);
            SsSeqStop(cur_seq_num);
            SsSeqClose(cur_seq_num);
            temp = gSDInfo;
        }
        gSDSeqNo = seq_req;
        seq_req = -1;
        temp = temp & ~0x100;
        temp = temp & ~0x1000;
        gSDSerialInfo = gSDSerialInfo & ~1;
        gSDInfo = temp;
        break;
    case 0xE1:
        if (!(gSDInfo & 0x100)) {
            return;
        }
        SsSeqPause(cur_seq_num);
        gSDInfo = (gSDInfo & ~0x100) | 0x1000;
        break;
    case 0xF1:
        if (!(gSDInfo & 0x1000)) {
            return;
        }
        SsSeqReplay(cur_seq_num);
        gSDInfo = (gSDInfo & ~0x1000) | 0x100;
        break;
    }
}
