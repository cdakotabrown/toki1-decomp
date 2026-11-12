/* @SymbolStart: 0x80150F20 */
/* @SymbolSize: 324 */
/* @RodataBase: 0x8015F1A0 */
/* Address: 0x80150F28 */
/* Size: 316 bytes (79 insns) */
#include "types.h"

extern void vram_area_clear(s32 a0);
extern void sub_sub_step_up(void);
extern void goto_sub_step(s32 a0);
extern void goto_sub_sub_step(s32 a0);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void game_reset();
extern void class_clear_all(s32 a0);
extern void debug_font_load(void);
extern void default_data_vram_load(void);
extern void boot_data_load(void);
extern void cal_font_load(s32 a0, s32 a1, s32 a2);
extern void cursor_init(s32 a0);
extern void set_flag_b3_high_bit(s32 a0);

extern u8 now_year;
extern u8 now_month;
extern u8 anim_fade;
extern u8 bunka_ret_sub_step;
extern u8 bunka_ret_sub_sub_step;
extern u8 now_sub_sub_step;

void bunkasai_maina3(void) {
    s32 t6 = now_sub_sub_step;
    s32 result;

    switch (t6) {
    case 0:
        class_clear_all(0);
        set_flag_b3_high_bit(0);
        vram_area_clear(255);
        sub_sub_step_up();
        break;
    case 1:
        boot_data_load();
        break;
    case 2:
        default_data_vram_load();
        break;
    case 3:
        debug_font_load();
        sub_sub_step_up();
        break;
    case 4:
        cd_read_start2(149, 0x801A0000, 0x9B94);
        sub_sub_step_up();
        break;
    case 5:
        result = get_CD_State2();
        if (result & 1) {
            cal_font_load(now_year, now_month, 0);
            sub_sub_step_up();
        }
        break;
    case 6:
        cursor_init(0);
        set_flag_b3_high_bit(0);
        anim_fade = 254;
        goto_sub_step(bunka_ret_sub_step);
        goto_sub_sub_step(bunka_ret_sub_sub_step);
        break;
    default:
        game_reset();
        break;
    }
}
