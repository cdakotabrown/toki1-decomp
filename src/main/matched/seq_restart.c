/* Function: seq_restart */
/* Address: 0x8007AE04 */
/* Size: 100 bytes (25 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x8007ADFC */
/* @SymbolSize: 108 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 gSDInfo;
extern s16 gSDSeqNo;

extern void SsUtAllKeyOff(s32);
extern void SsSetMVol(s32, s32);
extern void seq_ctrl(s32);
extern void seq_start_req(void);

void seq_restart(void) {
    s32 flags;

    flags = gSDInfo;
    if (((flags << 11) < 0) && !(flags & 0x400)) {
        SsUtAllKeyOff(0);
        SsSetMVol(0, 0);
    }

    seq_ctrl(113);

    if (gSDSeqNo != -1) {
        seq_start_req();
    }
}
