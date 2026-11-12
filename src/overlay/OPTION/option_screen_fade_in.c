/* @SymbolStart: 0x80139898 */
/* @SymbolSize: 148 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x1c:rt:v0:v1 0x20:rs:v0:v1 0x20:rt:t6:v0 0x24:rt:t6:v0 0x2c:rt:t6:v0 0x34:rt:t6:v0 0x64:rt:t6:v0 0x6c:rs:t6:v0 0x78:rt:t9:v1 0x80:rt:t9:v1 */
#include "types.h"
#include "tokimeki/libgs_types.h"
extern void k_disp_start();
extern void sub_sub_step_up();
extern u8 screen_bri;
extern LIBGS_BG Bg[];

void option_screen_fade_in(void) {
    u32 val;

    val = (u8)(screen_bri + 4);
    Bg[0].b = val;
    Bg[0].g = val;
    Bg[0].r = val;
    Bg[0].mx = 160;
    Bg[0].my = 120;
    Bg[0].x = 0;
    Bg[0].y = 0;
    screen_bri = val;
    if (val >= 128) {
        screen_bri = 128;
        k_disp_start(7);
        sub_sub_step_up();
    }
}
