/* @SymbolSize: 168 */
/* Address: 0x80146A20 */
/* Size: 168 bytes (42 insns) incl. alt-entry tails at 0x80146A40 / 0x80146A60 */
#include "types.h"

extern u8  taiiku_spoon_bal_0_down;
extern s32 pad_trig;
extern s32 taiiku_spoon_pl_press;
extern u8  taiiku_spoon_btn;
extern s32 taiiku_spoon_pl_spd;
extern s32 taiiku_spoon_pl_acc;
extern s32 taiiku_spoon_gain;

void taiiku_scroll_target(void)
{
    if (taiiku_spoon_bal_0_down != 0) {
        taiiku_spoon_pl_acc = -768;
        return;
    }
    if ((pad_trig & 0x20) != 0) {
        taiiku_spoon_btn = 1;
        return;
    }
    if ((u32)taiiku_spoon_pl_press >= 16U) {
        if (taiiku_spoon_btn != 0) {
            if (taiiku_spoon_pl_spd < 0) {
                taiiku_spoon_pl_spd = 0;
            }
            taiiku_spoon_pl_acc = taiiku_spoon_gain;
        } else {
            taiiku_spoon_pl_acc = -768;
        }
        taiiku_spoon_pl_press = 0;
        taiiku_spoon_btn = 0;
    }
}
