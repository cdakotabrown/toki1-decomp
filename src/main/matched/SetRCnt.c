/* Address: 0x800B5780 */
/* Size: 156 bytes (39 instructions) */
/* Region: Library (Psy-Q GCC) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -fno-schedule-insns2 --fill-load-delay */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: maspsx --aspsx-version=2.67 */
#include "types.h"

extern volatile s16 *libapi_rcnt_regs_ptr;

s32 SetRCnt(s32 a0, s32 a1, s32 a2)
{
    s32 idx;
    volatile s16 *p;
    s32 attr;

    idx = a0 & 0xffff;
    attr = 72;
    if (idx >= 3) {
        return 0;
    }
    p = libapi_rcnt_regs_ptr + idx * 8;
    p[2] = 0;
    p[4] = (s16)a1;
    if ((u32)idx < 2) {
        if (a2 & 0x10) {
            attr = 73;
        }
        if (!(a2 & 1)) {
            attr |= 0x100;
        }
    } else if (idx == 2) {
        if (!(a2 & 1)) {
            attr = 584;
        }
    }
    if (a2 & 0x1000) {
        attr |= 0x10;
    }
    p = libapi_rcnt_regs_ptr + idx * 8;
    p[2] = (s16)attr;
    return 1;
}
