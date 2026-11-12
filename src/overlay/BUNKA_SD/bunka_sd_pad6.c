/* @SymbolStart: 0x801362EC */
/* bunka_sd_pad6 - BUNKA_SD overlay, Address: 0x801362EC (head) / 0x801362F4, */
/* Size: 208 bytes (52 insns) */
/* @SymbolSize: 208 */
/* @CompileFlags: -Wo,-loopunroll,0 */
#include "types.h"

extern void bunka_sd_phase_disp4(void);
extern void goto_sub_sub_step(s32);

extern s32 pad_data;
extern u8 now_sub_sub_step;
extern u8 class_w[];

void bunka_sd_pad6(void)
{
    s32 i;

    if ((u32)now_sub_sub_step >= 3 && (u32)now_sub_sub_step < 7) {
        if (pad_data & 0x20) {
            bunka_sd_phase_disp4();
            for (i = 0; i < 3; i++) {
                class_w[i * 68 + 6534] = 130;
            }
        } else {
            for (i = 0; i < 3; i++) {
                class_w[i * 68 + 6534] = 1;
            }
        }
    }

    if ((u32)now_sub_sub_step >= 3 && (u32)now_sub_sub_step < 6) {
        if (pad_data & 0x40) {
            goto_sub_sub_step(5);
        }
    }

    bunka_sd_phase_disp4();
}
