/* Address: 0x80053A30 */
/* Size: 92 bytes (23 instructions) */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void DecDCTReset(s32 arg);
extern void DecDCToutCallback(s32 arg);
extern void DecDCTinCallback(s32 arg);

extern s32 mdec_rl;
extern s32 mdec_image;
extern s32 mdec_bs;

void dec_env_set(s32 arg0, s32 arg1, s32 arg2) {
    DecDCTReset(0);
    DecDCToutCallback(0);
    DecDCTinCallback(0);
    mdec_rl = arg0;
    mdec_image = arg1;
    mdec_bs = arg2;
}
