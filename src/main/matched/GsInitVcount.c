/* Likely Symbol: GsClearOt (from o.bin, offset +80 bytes from 0x800995E0) */
/* Address: 0x80099590 */
/* Size: 56 bytes (14 instructions) */
/* Region: Library (>= 0x80086000) */
/* @Compiler: gcc260 */
/* @CompileFlags: --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void SetRCnt(u32 arg0, s32 arg1, s32 arg2);
extern void StartRCnt(u32 arg0);

void GsInitVcount(void) {
    SetRCnt(0xF2000001, -1, 8192);
    StartRCnt(0xF2000001);
}
