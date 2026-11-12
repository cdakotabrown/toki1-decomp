/* Size: 396 bytes */
/* @SymbolSize: 396 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x90 --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x94 --offset2 0xa0 */
/* @PostProc: swap_instructions --offset1 0x9c --offset2 0xa0 */
/* @PostProc: swap_instructions --offset1 0xa0 --offset2 0xa4 */
/* @PostProc: swap_registers a0:v0 --unidirectional */
/* @PostProc: swap_registers a1:a0 --unidirectional */
/* @PostProc: swap_registers a2:a1 --unidirectional */
/* @PostProc: swap_registers a3:a2 --unidirectional */
/* @PostProc: swap_registers t9:v1 --unidirectional */
/* @PostProc: swap_registers t0:t9 --unidirectional */
/* @PostProc: swap_registers t1:t0 --unidirectional */
/* @PostProc: swap_registers t2:t1 --unidirectional */
/* @PostProc: swap_registers t3:t2 --unidirectional */
#include "types.h"

extern s32 taiiku_tama_fall;
extern s32 taiiku_tama_pstep;
extern s32 taiiku_tama_gstep;
extern u16 taiiku_tama_bal_tick;
extern s32 taiiku_tama_push;
extern s32 taiiku_tama_push_1;
extern s32 taiiku_tama_vel;
extern s32 taiiku_tama_vel_1;
extern s32 taiiku_tama_ppush;
extern s32 taiiku_tama_gpush;

void taiiku_balance_advance(void) {
    s32 x;
    s32 y;

    if (taiiku_tama_fall != 0 && taiiku_tama_pstep == 4 && taiiku_tama_gstep == 4) {
        taiiku_tama_vel_1 = 0;
        if (taiiku_tama_fall == 1) {
            taiiku_tama_vel = -0x10000;
        } else {
            taiiku_tama_vel = 0x10000;
        }
        return;
    }

    if ((u32)taiiku_tama_bal_tick >= 11) {
        y = taiiku_tama_vel_1;
        taiiku_tama_bal_tick = 0;
        y = y + taiiku_tama_push_1;
        if (y < 0) {
            y = 0;
        }
        if (y > 0x10000) {
            y = 0x10000;
        }

        x = taiiku_tama_vel;
        if (x > 0) {
            x = x + taiiku_tama_push;
            if (x <= 0) {
                taiiku_tama_ppush = 0;
                taiiku_tama_gpush = 0;
                taiiku_tama_push = 0;
                x = 0;
            }
        } else if (x < 0) {
            x = x + taiiku_tama_push;
            if (x >= 0) {
                taiiku_tama_ppush = 0;
                taiiku_tama_gpush = 0;
                taiiku_tama_push = 0;
                x = 0;
            }
        } else {
            x = x + taiiku_tama_push;
        }

        if (x < -0x10000) {
            x = -0x10000;
        }
        taiiku_tama_vel = x;
        if (x > 0x10000) {
            taiiku_tama_vel = 0x10000;
        }

        if (y < -0x10000) {
            y = -0x10000;
        }
        taiiku_tama_vel_1 = y;
        if (y > 0x10000) {
            taiiku_tama_vel_1 = 0x10000;
        }
    }
}
