/* @SymbolStart: 0x80137CAC */
/* bunka_sd_pad8 - BUNKA_SD overlay, Address: 0x80137CB4, Size: 184 bytes (46 insns) */
/* @SymbolSize: 192 */
#include "types.h"

extern s32 get_CD_State(void);
extern void device_cmd_send(s32);
extern void goto_sub_sub_step(s32);
extern void bunka_sd_cue_dispatch(void);

extern s32 pad_data;
extern u8 now_sub_sub_step;

void bunka_sd_pad8(void)
{
    s32 v0;

    v0 = now_sub_sub_step;
    if ((u32)v0 >= 3 && (u32)v0 < 47) {
        if (pad_data & 0x20) {
            if (get_CD_State() != 2) {
                device_cmd_send(116);
            }
            bunka_sd_cue_dispatch();
            v0 = now_sub_sub_step;
        }
    }

    if ((u32)v0 >= 3 && (u32)v0 < 45) {
        if (pad_data & 0x40) {
            device_cmd_send(193);
            device_cmd_send(196);
            goto_sub_sub_step(44);
        }
    }

    bunka_sd_cue_dispatch();
}
