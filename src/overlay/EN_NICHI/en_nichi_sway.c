/* @SymbolStart: 0x801356E8 */
/* @SymbolSize: 148 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x68 */
/* en_nichi_sway - EN_NICHI overlay, Address: 0x801356E8, Size: 148 bytes (37 insns) */
#include "types.h"

extern void en_nichi_grid_set();
extern s32 rsin();
extern s32 rcos();
extern void en_nichi_marker_ripple();
extern void en_nichi_grid_disp();
extern volatile s32 en_nichi_sway_cnt;
extern volatile s32 en_nichi_sway_ang;
extern volatile s32 en_nichi_sway_x;
extern volatile s32 en_nichi_sway_y;

void en_nichi_sway(void)
{
    s32 a0;
    s32 v0;

    en_nichi_sway_cnt = en_nichi_sway_cnt + 1;
    en_nichi_grid_set();
    a0 = en_nichi_sway_ang;
    a0 += 3;
    if (a0 >= 4096) {
        a0 = 0;
    }
    en_nichi_sway_ang = a0;
    v0 = rsin(a0);
    v0 = (v0 << 3) >> 12;
    a0 = en_nichi_sway_ang;
    en_nichi_sway_x = v0;
    v0 = rcos(a0);
    en_nichi_sway_y = (v0 << 4) >> 12;
    en_nichi_marker_ripple();
    en_nichi_grid_disp();
}
