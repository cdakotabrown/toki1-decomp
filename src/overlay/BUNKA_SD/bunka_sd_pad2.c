/* @SymbolStart: 0x801336AC */
/* @SymbolSize: 192 */
#include "types.h"

extern s32 get_CD_State(void);
extern void device_cmd_send(s32);
extern void bunka_sd_step_dispatch(void);
extern void goto_sub_sub_step(s32);

extern s32 pad_data;
extern u8 now_sub_sub_step;

void bunka_sd_pad2(void)
{
    s32 v0_in;
    s32 ret;

    v0_in = now_sub_sub_step;

    if ((u32)v0_in >= 3 && (u32)v0_in < 40 && (pad_data & 0x0020)) {
        ret = get_CD_State();
        if (ret != 2) {
            device_cmd_send(116);
        }
        bunka_sd_step_dispatch();
        v0_in = now_sub_sub_step;
    }

    if ((u32)v0_in >= 3 && (u32)v0_in < 38 && (pad_data & 0x0040)) {
        device_cmd_send(193);
        device_cmd_send(196);
        goto_sub_sub_step(37);
    }
    bunka_sd_step_dispatch();
}
