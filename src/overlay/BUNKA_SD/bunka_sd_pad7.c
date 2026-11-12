/* @SymbolStart: 0x80136B3C */
/* bunka_sd_pad7 - BUNKA_SD overlay, Address: 0x80136B44, Size: 184 bytes (46 insns) */
/* @SymbolSize: 192 */
#include "types.h"

extern s32 get_CD_State(void);
extern void device_cmd_send(s32);
extern void goto_sub_sub_step(s32);
extern void bunka_sd_step_disp(void);

extern s32 pad_data;
extern u8 now_sub_sub_step;

void bunka_sd_pad7(void)
{
    s32 v0;

    v0 = now_sub_sub_step;
    if ((u32)v0 >= 3 && (u32)v0 < 38) {
        if (pad_data & 0x20) {
            if (get_CD_State() != 2) {
                device_cmd_send(116);
            }
            bunka_sd_step_disp();
            v0 = now_sub_sub_step;
        }
    }

    if ((u32)v0 >= 3 && (u32)v0 < 36) {
        if (pad_data & 0x40) {
            device_cmd_send(193);
            device_cmd_send(196);
            goto_sub_sub_step(35);
        }
    }

    bunka_sd_step_disp();
}
