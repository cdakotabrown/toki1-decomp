/* @SymbolStart: 0x801444B8 */
/* @SymbolSize: 344 */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xd8 */
/* @PostProc: patch_insn_reg_field 0x4c:rt:v0:v1 0x54:rs:v0:v1 0x54:rt:v0:v1 0x5c:rt:v1:v0 0x60:rs:v0:v1 0x70:rt:v0:v1 0x74:rs:v1:v0 */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xd4 */
/* @PostProc: swap_instructions --offset1 0xcc --offset2 0xd0 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xcc */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xc4 */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xc0 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xbc */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xc0 */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xc4 */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xcc */
/* @PostProc: swap_instructions --offset1 0xcc --offset2 0xd0 */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xd4 */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xd8 */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xc0 */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xc4 */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xcc */
/* @PostProc: swap_instructions --offset1 0xcc --offset2 0xd0 */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xd4 */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xd8 */
#include "types.h"

extern void debug_hex_show(s32 a0, s32 a1, s32 a2);
extern void scroll_poly_disp(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);

extern u8 taiiku_ninin_tick;
extern s32 taiiku_ninin_vmax;
extern s32 taiiku_ninin_run_0_accel;
extern s32 taiiku_ninin_scrl;
extern s16 taiiku_ninin_scrl_hi;
extern s32 taiiku_ninin_scrv;
extern s16 cw100_frame;

void taiiku_race_scroll_disp(void)
{
    s32 v0;
    s16 h;

    if ((u32)taiiku_ninin_tick >= 11U) {
        taiiku_ninin_scrv = taiiku_ninin_scrv + taiiku_ninin_run_0_accel;
        taiiku_ninin_tick = 0;
    }
    if (taiiku_ninin_vmax < taiiku_ninin_scrv) {
        taiiku_ninin_scrv = taiiku_ninin_vmax;
    }
    v0 = taiiku_ninin_scrv;
    if (v0 < 0) {
        v0 = 0;
    }
    h = cw100_frame;
    if (h == 45 || h == 57) {
        v0 = 0;
    }
    taiiku_ninin_scrl = taiiku_ninin_scrl + v0;
    taiiku_ninin_scrv = v0;
    scroll_poly_disp((taiiku_ninin_scrl_hi / 4) % 640, 15, 0, 112, 640);
    scroll_poly_disp((taiiku_ninin_scrl_hi / 2) % 128, 15, 112, 128, 128);
    debug_hex_show(taiiku_ninin_scrv, 0, -60);
}
