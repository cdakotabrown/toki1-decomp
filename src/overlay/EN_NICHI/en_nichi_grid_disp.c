/* en_nichi_grid_disp - EN_NICHI overlay, Address: 0x801360D0, Size: 148 bytes (37 insns) */
/* @SymbolSize: 104 */
/* @PostProc: swap_registers s1:s2 --bidirectional */
/* @PostProc: scheduler_normalizer */
/* @PostProc: remap_stack_slots --from 24 --to 28 --from 28 --to 24 */
#include "types.h"

extern void en_nichi_grid_quad_disp();

void en_nichi_grid_disp(void)
{
    s32 a0;
    s32 a1;
    s32 s0;
    s32 s1;
    s32 s2;
    s32 s3;

    s1 = 14;
    s2 = 0;
    s3 = 11;
    s0 = 0;

    do {
        a0 = s0;
        a1 = s2;
        en_nichi_grid_quad_disp(a0, a1);
        s0 += 1;
        if (s0 == s1) {
            s0 = 0;
            s2 += 1;
            if (s2 == s3) {
                return;
            }
        }
    } while (1);
}
