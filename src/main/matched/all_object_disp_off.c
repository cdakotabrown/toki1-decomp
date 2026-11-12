/* @CompileFlags: -O2 -G0 -mips1 -EL -non_shared -Wo,-loopunroll,0 */
/* Address: 0x800593C8, Size: 52 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: rename_register_chain --chain v0:v1:a0:v0 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
#include "types.h"
extern s32 ref_view[];
extern s32 object[];
void all_object_disp_off(void) {
    s32 *p = object;
    s32 *end = ref_view;
    do {
        p += 16;
        p[-12] = (s32)0x80000000;
        p[-8] = (s32)0x80000000;
        p[-4] = (s32)0x80000000;
        p[-16] = (s32)0x80000000;
    } while (p != end);
}
