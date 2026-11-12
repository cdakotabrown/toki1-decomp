/* Address: 0x8007BF94 */
/* Size: 164 bytes (41 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u16 sd_vab_ch0;
extern s32 gSDInfo;
extern s16 cur_seq_num;
extern s16 gSDSeqNo;
extern s16 seq_req;
extern u16 sd_vab_ch2;

extern void SsSeqStop(s16);
extern void SsSeqClose(s16);
extern void SsUtAllKeyOff(s32);
extern void SsVabClose(s16);

void SD_VABClose(s16 arg0) {
    if ((s16)(u16)arg0 == sd_vab_ch0) {
        if (gSDInfo & 0x100) {
            SsSeqStop(cur_seq_num);
            SsSeqClose(cur_seq_num);
        }
        gSDSeqNo = -1;
        seq_req = -1;
    } else if ((s16)(u16)arg0 == sd_vab_ch2) {
        SsUtAllKeyOff(0);
    }
    SsVabClose(arg0);
}
