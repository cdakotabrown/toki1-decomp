/* @SymbolStart: 0x80132004 */
/* @SymbolSize: 176 */
/* @PostProc: rename_register_chain --chain t7:t6:v1 */
/* @PostProc: rename_register_chain --chain t0:t9:t8:t7 */
/* @PostProc: swap_registers a0:v0,a1:v1 */
/* @PostProc: patch_insn_reg_field 0x8c:rt:v0:a0 0x94:rt:v1:a1 */
/* @PostProc: patch_insn_reg_field 0x98:rd:v0:a0 */
/* @PostProc: patch_insn_reg_field 0x9c:rd:v1:a1 */
/* Address: 0x8013200C, Size: 192 bytes (48 instructions) */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern u8 screen_bri;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern void _sprite_set_box_shade();
extern void dtd_on_tpage();
extern u8 now_sub_sub_step;
extern LIBGS_BG Bg[];

void option_cal_box_show(void) {
    s32 hidden_v0;
    s16 sv0;
    s16 sv1;

    hidden_v0 = now_sub_sub_step;

    if (hidden_v0 == 33 || hidden_v0 == 34 || hidden_v0 == 35) {
        return;
    }
    if (screen_bri == 0) {
        return;
    }
    if ((u32)Bg[0].r < 127) {
        return;
    }
    sv0 = cal_cw_x;
    sv1 = cal_cw_y;
    _sprite_set_box_shade((s32)sv0 + 24, (s32)sv1 + 16, 124, 188, 10, 0x00010907, 131);
    dtd_on_tpage(0, 0, 10, 0, 0);
}
