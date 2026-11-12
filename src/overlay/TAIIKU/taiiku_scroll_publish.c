/* @SymbolSize: 204 */
/* Address: 0x80146B30 */
/* Size: 204 bytes (51 insns) */
#include "types.h"

extern s16 cal_cw_frame;
extern u8  taiiku_spoon_bal_0_down;
extern s32 taiiku_spoon_scrv;
extern s32 taiiku_spoon_pl_acc;
extern s16 taiiku_spoon_scrl_hi;
extern s32 taiiku_spoon_pl_pos;
extern volatile s16 taiiku_spoon_pl[];
extern s16 cal_cw_x;

#define taiiku_spoon_pl_x taiiku_spoon_pl[13]

void taiiku_scroll_publish(void)
{
    s16 v;

    if (cal_cw_frame == 3) {
        goto clamp;
    }
    if (taiiku_spoon_bal_0_down != 0) {
        if (taiiku_spoon_scrv == 0) {
            goto clamp;
        }
    }
    if (taiiku_spoon_pl_acc < 0) {
        goto sub;
    }
    if (taiiku_spoon_scrl_hi != 0) {
        goto add;
    }
sub:
    taiiku_spoon_pl_pos -= taiiku_spoon_scrv;
    goto clamp;
add:
    taiiku_spoon_pl_pos += 0x8000;
clamp:
    v = taiiku_spoon_pl_x;
    if (v >= -31) {
        taiiku_spoon_pl_x = -32;
        v = taiiku_spoon_pl_x;
    }
    if (v < -159) {
        taiiku_spoon_pl_x = -160;
        v = taiiku_spoon_pl_x;
    }
    cal_cw_x = v;
}
