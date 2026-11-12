/* Function: seq_cmd */
/* Address: 0x8007AE6C  -> window now starts at 0x8007AE68 */
/* Size: 228 bytes (57 instructions)  -> window 232 bytes / 58 insns */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileEnv: IDO52_PRECOLOR_WEB=a1:5 */
/* @SymbolStart: 0x8007AE68 */
/* @SymbolSize: 232 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u32 gSDInfo;
extern u32 gSDSerialInfo;
extern s16 gSDSeqNo;
extern s16 seq_req;

extern void seq_restart(void);
extern void seq_start_req(void);
extern void seq_ctrl(s32 arg0);
extern void seq_vol_fade_set(s32 arg0);

void seq_cmd(s32 arg0) {
    u32 val;
    u32 sel;

    val = arg0 & 0xFFFF;
    sel = val & 0xF000;
    switch (sel) {
    case 0:
        if (gSDSerialInfo & 1) {
            seq_req = (s16)(val & 0xFF);
            seq_restart();
            return;
        }
        if (gSDInfo & 0x100) {
            seq_req = (s16)(val & 0xFF);
            seq_vol_fade_set(177);
            return;
        }
        gSDSeqNo = (s16)(val & 0xFF);
        seq_req = -1;
        seq_start_req();
        return;

    case 0x1000:
        seq_ctrl(113);
        return;

    case 0x2000:
        seq_ctrl(225);
        return;

    case 0x4000:
        seq_ctrl(241);
        return;
    }
}
