/* @SymbolStart: 0x801322EC */
/* bunka_sd_pad1 - BUNKA_SD overlay, Address: 0x801322F4, Size: 308 bytes (77 insns) */
/* @SymbolSize: 316 */
#include "types.h"

typedef struct {
    u8 pad0[6];
    u8 f6;
    u8 pad7[61];
} Rec68;

extern Rec68 class_w[];

extern s32 get_CD_State(void);
extern void device_cmd_send(s32);
extern void bunka_sd_step(void);
extern void goto_sub_sub_step(s32);

extern s32 pad_data;
extern u8 now_sub_sub_step;

void bunka_sd_pad1(void)
{
    u32 idx;
    s32 i;

    idx = now_sub_sub_step;

    if (idx >= 5 && idx < 15) {
        if (pad_data & 0x20) {
            if (get_CD_State() != 2) {
                device_cmd_send(116);
            }
            bunka_sd_step();
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

    if (idx >= 5 && idx < 14) {
        if (pad_data & 0x40) {
            device_cmd_send(196);
            goto_sub_sub_step(13);
        }
    }
    bunka_sd_step();
}
