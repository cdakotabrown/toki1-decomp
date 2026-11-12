/* Address: 0x800B4200 */
/* Size: 88 bytes (22 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 PadIdentifier;
extern s32 pad_buf;

void ResetCallback(void);
s32 PAD_init(u32 mode, s32 *result);
void ChangeClearPAD(s32 arg);

s32 PadInit(s32 arg0) {
    s32 result;

    PadIdentifier = arg0;
    pad_buf = -1;
    ResetCallback();
    result = PAD_init(0x20000001, &pad_buf);
    ChangeClearPAD(0);
    return result;
}
