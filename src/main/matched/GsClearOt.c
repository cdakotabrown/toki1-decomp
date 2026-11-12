/* Address: 0x800995E0 */
/* Size: 88 bytes (22 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O1 --konami-lib-sched */
/* @PostProc: maspsx --aspsx-version=2.67 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: fill_gcc_delay_slots */
#include "types.h"

extern void ClearOTagR(s32, s32);

void GsClearOt(s32 arg0, s32 arg1, s32 *arg2) {
    s32 shift;
    s32 base;
    s32 tmp;

    arg0 = arg0 & 0xFFFF;
    shift = arg2[0];
    arg2[2] = arg0;
    base = arg2[1];
    tmp = 4;
    tmp = tmp << shift;
    arg1 = arg1 & 0xFFFF;
    tmp = tmp + base;
    tmp = tmp - 4;
    arg2[3] = arg1;
    arg2[4] = tmp;
    arg1 = 1 << arg2[0];
    ClearOTagR(arg2[1], arg1);
}
