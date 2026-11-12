/* Function: SD_Exit */
/* Address: 0x8007BE78 */
/* Size: 172 bytes (43 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x8007BE78 */
/* @SymbolSize: 172 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s16 cur_seq_num;
extern s32 gSDInfo;
extern void SsSeqStop(s16 a0);
extern void SsSeqClose(s16 a0);
extern void SsUtAllKeyOff(s32 a0);
extern void SsVabClose(s16 a0);
extern void SsEnd(void);
extern void SsQuit(void);

void SD_Exit(void) {
    s32 check = gSDInfo;
    s16 var_s0;

    if (check & 0x1100) {
        SsSeqStop(cur_seq_num);
        SsSeqClose(cur_seq_num);
    }
    SsUtAllKeyOff(0);
    for (var_s0 = 3; var_s0 >= 0; var_s0--) {
        if ((0x10000 << var_s0) & gSDInfo) {
            SsVabClose(var_s0);
        }
    }
    SsEnd();
    SsQuit();
}
