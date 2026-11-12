/* Function: v_wait */
/* Address: 0x800B4164 */
/* Size: 152 bytes (38 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -fno-delayed-branch -fno-schedule-insns -fno-schedule-insns2 --fill-earliest */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions 0x5c 0x64 */
/* `@PostProc: custom_script custom_GsTMDfastTNF3.py`, now retired and deleted. */
#include "types.h"

extern s32 Vcount;
extern void puts(void *addr);
extern void ChangeClearPAD(s32 arg0);
extern void ChangeClearRCnt(s32 arg0, s32 arg1);

extern s8 v_wait_tbl0;

void v_wait(s32 arg0, s32 arg1) {
    volatile s32 loop_count;
    s32 neg;

    loop_count = arg1 << 15;

    if (Vcount >= arg0) {
        return;
    }
    neg = -1;

    while (1) {
        if ((loop_count = loop_count - 1) == neg) {
            puts(&v_wait_tbl0);
            ChangeClearPAD(0);
            ChangeClearRCnt(3, 0);
            break;
        }
        if (Vcount >= arg0) {
            break;
        }
    }
}
