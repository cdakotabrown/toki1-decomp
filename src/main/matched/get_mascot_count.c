/* Address: 0x8005DFF4 */
/* Size: 236 bytes (59 instructions) */
/* Region: Game (leaf - the target has no prologue, so no frame directive) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=a1:16,a0:13 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions 0x34 0x38 */
#include "types.h"

extern u8 mascot_flag;
extern u8 mascot_flag2;
extern u8 album_work_1[];
extern u8 D_800EE761[];
extern u8 mascot_flag_end[];

s32 get_mascot_count(void) {
    s32 count = 0;
    u8 *pa;
    u8 *pb;

    if (mascot_flag) count++;
    if (mascot_flag2) count++;
    pb = D_800EE761;
    pa = album_work_1;
    do {
        if (pa[0x14]) count++;
        if (pb[0x53C]) count++;
        if (pa[0x15]) count++;
        if (pb[0x53D]) count++;
        if (pa[0x16]) count++;
        if (pb[0x53E]) count++;
        if (pa[0x17]) count++;
        if (pb[0x53F]) count++;
        pa += 4;
        pb += 4;
    } while (pa != mascot_flag_end);
    return count;
}
