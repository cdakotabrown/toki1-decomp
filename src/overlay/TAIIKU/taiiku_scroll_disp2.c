/* Address: 0x80146DD8 (true entry 0x80146DBC -- cross-prologue head) */
/* @SymbolStart: 0x80146DBC */
/* @SymbolSize: 248 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0xa0 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x90 --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x94 --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x98 --offset2 0x9c */
/* @PostProc: swap_instructions --offset1 0x9c --offset2 0xa0 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rt:t6:v1 0x1c:rs:t6:v0 0x1c:rt:t6:v0 0x28:rs:t6:v0 0x44:rt:t8:t7 0x4c:rt:t7:t6 0x50:rs:t7:t6 0x50:rt:t7:t6 0x58:rs:t7:t6 0x68:rs:t8:t7 0x68:rt:t8:t7 0x70:rd:t9:t8 0x70:rs:t8:t7 0x74:rt:t9:t8 0x80:rt:t2:t1 0x88:rd:t0:t9 0x90:rd:t0:t9 0x98:rs:t0:t9 0xa4:rt:t2:t1 0xc8:rt:t4:t3 0xd0:rt:t3:t2 0xd4:rs:t3:t2 0xdc:rs:t3:t2 0xdc:rt:t3:t2 0xe0:rs:t3:t2 0xe4:rt:t4:t3 */
#include "types.h"

extern s32 taiiku_spoon_vmax;
extern s32 taiiku_spoon_scrl;
extern volatile s32 taiiku_spoon_scrv;
extern s32 taiiku_spoon_pl_acc;
extern s16 taiiku_spoon_scrl_hi;
extern s16 taiiku_spoon_pl_x;

extern s32 scroll_poly_disp();

void taiiku_scroll_disp2(void) {
    s32 v0;
    s32 v1;
    s32 a0;
    s16 sv;
    s32 half;
    s32 tmp;
    s32 q;

    v1 = taiiku_spoon_pl_acc;
    v0 = taiiku_spoon_scrv + v1;
    a0 = taiiku_spoon_vmax;
    if (a0 < v0) {
        v0 = a0;
    }
    if (v0 < 0) goto L_zero;
    if (v1 >= 0) goto L_after;
    if (taiiku_spoon_pl_x >= -159) goto L_after;
L_zero:
    v0 = 0;
L_after:
    taiiku_spoon_scrl = taiiku_spoon_scrl + v0;
    sv = taiiku_spoon_scrl_hi;
    half = sv / 2;
    q = half % 640;
    taiiku_spoon_scrv = v0;
    scroll_poly_disp(q, 15, 0, 112, 640);
    sv = taiiku_spoon_scrl_hi;
    tmp = sv % 128;
    scroll_poly_disp(tmp, 15, 112, 128, 128);
}
