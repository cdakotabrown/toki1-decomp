/* Likely Symbol: def_cbsync (from o.bin, offset +48 bytes from 0x800877F8) */
/* Address: 0x80087828 - 0x80087850 (40 bytes) */
/* Debug Symbol: def_cbsync (from o.bin, high match) */
/* Region: Library (>= 0x80086000) */
/* @Compiler: gcc260 */
/* @CompileFlags: --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void DeliverEvent(u32 arg0, s32 arg1);

void def_cbsync(void) {
    DeliverEvent(0xF0000003, 32);
}
