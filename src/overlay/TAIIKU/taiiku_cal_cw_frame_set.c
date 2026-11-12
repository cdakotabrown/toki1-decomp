/* @SymbolSize: 232 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x24 */
/* @PostProc: patch_insn_reg_field 0xc4:rt:t9:t1 0xe4:rt:t9:t1 */
#include "types.h"

struct { volatile s32 unk0; u16 unk4; u16 pad6; } taiiku_tama_pfrm = {0};
extern s32 taiiku_tama_fall;
extern s32 taiiku_tama_pstep;
extern s32 taiiku_tama_btn;
extern s16 cal_cw_frame;

void taiiku_cal_cw_frame_set(void)
{
    s32 v;

    if ((u32)taiiku_tama_pfrm.unk4 >= 11U) {
        v = taiiku_tama_pfrm.unk0;
        if (v >= 3 && taiiku_tama_fall != 0 &&
            (taiiku_tama_pstep == 2 || taiiku_tama_pstep == 4 || taiiku_tama_pstep == 6)) {
            v = 0;
        } else if (v >= 5) {
            v = 0;
        } else {
            v = v + 1;
        }
        taiiku_tama_btn = 0;
        taiiku_tama_pfrm.unk4 = 0;
        taiiku_tama_pfrm.unk0 = v;
    }

    v = taiiku_tama_pfrm.unk0;
    cal_cw_frame = (s16)v;
    if (taiiku_tama_fall == 0) {
        return;
    }
    if (taiiku_tama_pstep == 2 || taiiku_tama_pstep == 4) {
        goto blk;
    }
    taiiku_tama_pfrm.unk0 = v;
    if (taiiku_tama_pstep != 6) {
        return;
    }
blk:
    cal_cw_frame = (s16)(v + 72);
    taiiku_tama_pfrm.unk0 = v;
}
