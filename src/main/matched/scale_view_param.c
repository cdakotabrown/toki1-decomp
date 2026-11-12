/* Function: scale_view_param */
/* Address: 0x8009A924 */
/* Size: 240 bytes (60 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --fill-epilogue --delay-slot-load-nop */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0xe0 --offset2 0xe4 */
#include "types.h"

extern s32 select_max_param(s32 *);
extern s32 len_param(s32);

void scale_view_param(s32 *arg0, s32 *arg1) {
    s32 result;
    s32 shift;

    result = select_max_param(arg0);
    result = len_param(result);

    if (result >= 16) {
        shift = result - 15;
        arg1[0] = arg0[0] >> shift;
        arg1[1] = arg0[1] >> shift;
        arg1[2] = arg0[2] >> shift;
        arg1[3] = arg0[3] >> shift;
        arg1[4] = arg0[4] >> shift;
        arg1[5] = arg0[5] >> shift;
    } else {
        arg1[0] = arg0[0];
        arg1[1] = arg0[1];
        arg1[2] = arg0[2];
        arg1[3] = arg0[3];
        arg1[4] = arg0[4];
        arg1[5] = arg0[5];
    }
}
