/* @SymbolStart: 0x801351DC */
/* bunka_sd_pad4 - BUNKA_SD overlay, Address: 0x801351E4, Size: 268 bytes (67 insns) */
/* @SymbolSize: 280 */
#include "types.h"

typedef struct {
    u8 pad0[6];
    u8 f6;
    u8 pad7[61];
} Rec68;

extern Rec68 class_w[];

extern void bunka_sd_phase_disp2(void);
extern void goto_sub_sub_step(s32);

extern s32 pad_data;
extern u8 now_sub_sub_step;

void bunka_sd_pad4(void)
{
    u32 idx;
    s32 i;

    idx = now_sub_sub_step;

    if (idx >= 3 && idx < 9) {
        if (pad_data & 0x20) {
            bunka_sd_phase_disp2();
            for (i = 0; i < 10; i++) {
                class_w[i + 96].f6 = 130;
            }
            idx = now_sub_sub_step;
        } else {
            for (i = 0; i < 10; i++) {
                class_w[i + 96].f6 = 1;
            }
        }
    }

    if (idx >= 3 && idx < 8) {
        if (pad_data & 0x40) {
            goto_sub_sub_step(7);
        }
    }
    bunka_sd_phase_disp2();
}
