/* @SymbolStart: 0x80138ADC */
/* @SymbolSize: 96 */
/* bunka_sd_pad9 - BUNKA_SD overlay, Address: 0x80138ADC, Size: 96 bytes (24 insns) */
#include "types.h"

extern u8 now_sub_sub_step;
extern s32 pad_data;
extern void goto_sub_sub_step(s32);
extern void device_cmd_send(s32);
extern void bunka_sd_xa_dispatch(void);

void bunka_sd_pad9(void)
{
    u8 sel;
    s32 ctrl;

    sel = now_sub_sub_step;
    if ((u32)sel < 3U) {
        goto last;
    }
    if ((u32)sel >= 6U) {
        goto last;
    }
    ctrl = pad_data;
    if (!(ctrl & 0x40)) {
        goto last;
    }
    goto_sub_sub_step(5);
    device_cmd_send(196);
last:
    bunka_sd_xa_dispatch();
}
