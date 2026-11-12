/* Address: 0x8004EEFC */
/* Size: 40 bytes (10 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "common.h"

extern s16 k_sub_reset_point;
extern void k_disp_start(s32 arg0);

void k_sub_disp_start(s32 a0) {
    k_disp_start((s32)k_sub_reset_point + a0);
}
