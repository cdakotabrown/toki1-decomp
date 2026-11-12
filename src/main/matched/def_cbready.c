/* Likely Symbol: CdReset (from o.bin, offset +76 bytes from 0x8008789C) */
/* Address: 0x80087850 - 0x80087878 (40 bytes) */
/* Region: Library (>= 0x80086000) */
/* @Compiler: gcc260 */
/* @CompileFlags: --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void DeliverEvent(u32 arg0, s32 arg1);

void def_cbready(void) {
    DeliverEvent(0xF0000003, 64);
}
