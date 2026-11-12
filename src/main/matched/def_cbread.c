/* Likely Symbol: CdReset (from o.bin, offset +36 bytes from 0x8008789C) */
/* Address: 0x80087878 - 0x800878A0 (40 bytes) */
/* Region: Library (>= 0x80086000) */
/* @Compiler: gcc260 */
/* @CompileFlags: --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void DeliverEvent(u32, s32);

void def_cbread(void) {
    DeliverEvent(0xF0000003u, 64);
}
