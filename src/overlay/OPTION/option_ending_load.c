/* @SymbolStart: 0x8013871C */
/* @SymbolSize: 160 */
/* Overlay: OPTION */
/* Address: 0x80138724 */
/* Size: 152 bytes (38 instructions) */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern u8 screen_bri;
extern LIBGS_BG Bg[];
extern s32 device_load_poll();
extern void class_clear_f();
extern void prim_init();
extern void cd_read_start2();
extern void sub_sub_step_up();

void option_ending_load(void) {
    s32 hidden_v0;
    u8 v1;
    s32 v0_byte;

    hidden_v0 = screen_bri;

    if ((u32)hidden_v0 >= 3) {
        screen_bri = (u8)(hidden_v0 - 2);
    } else {
        class_clear_f();
        prim_init();
    }

    v1 = Bg[0].r;
    v0_byte = v1 - 2;
    if ((u32)v1 >= 3) {
        Bg[0].b = (u8)v0_byte;
        Bg[0].g = (u8)v0_byte;
        Bg[0].r = (u8)v0_byte;
    }

    if (device_load_poll() == 1) {
        cd_read_start2(17, (s32)0x801F0000, 0xAE07);
        sub_sub_step_up();
    }
}
