/* @SymbolStart: 0x80132320 */
/* @SymbolSize: 396 */
/* @PostProc: patch_insn_reg_field 0xe0:rt:v0:a0 0xe4:rs:v0:a0 0xe4:rt:v0:a0 0xec:rs:v0:a0 0xec:rt:v0:a0 0xf0:rs:v0:a0 0xf0:rt:t2:a1 0xf4:rt:t2:a1 0xfc:rt:t2:a1 0x104:rt:t2:a1 0x10c:rt:t2:a1 0x114:rt:t2:a1 0x118:rs:t2:a1 */
/* name_ent_fade_in - NAME_ENT overlay, Address: 0x80132328, Size: 388 bytes (97 insns) */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern s32 *name_ent_kanji_txt;
extern u8 step_mode;
extern u8 screen_bri;
extern u8 fade_bri;
extern u32 Bg;

extern s32 get_CD_State2();
extern void name_ent_bg_init();
extern void vram_full_move();
extern void name_ent_layout_init();
extern void set_flag_b3_high_bit();
extern void back_clear_switch();
extern void sub_step_up();
extern void debug_hex_show();
extern u8 step_mode;

void name_ent_fade_in(void)
{
    s32 *arr;
    s32 i;
    s32 t0;
    u32 a1;

    if (step_mode == 0) {
        if (get_CD_State2() & 1) {
            arr = name_ent_kanji_txt;
            for (i = 0; i < 50; i++) {
                arr[i] = arr[i] + 0x801E0000;
            }
            name_ent_bg_init();
            ((LIBGS_BG *)&Bg)[0].g = 0;
            t0 = step_mode;
            ((LIBGS_BG *)&Bg)[0].r = 0;
            step_mode = t0 + 1;
            ((LIBGS_BG *)&Bg)[0].b = 0;
            screen_bri = 0;
        }
    } else {
        a1 = (fade_bri + 4) & 0xFF;
        ((LIBGS_BG *)&Bg)[0].g = a1;
        ((LIBGS_BG *)&Bg)[0].r = a1;
        fade_bri = a1;
        ((LIBGS_BG *)&Bg)[0].b = a1;
        screen_bri = a1;
        if (a1 >= 128) {
            vram_full_move(240);
            name_ent_layout_init();
            Bg = 0x80000000;
            ((LIBGS_BG *)&Bg)[0].x = -24;
            set_flag_b3_high_bit(1);
            back_clear_switch(0);
            sub_step_up();
        }
    }
    debug_hex_show(fade_bri, 0, 0);
    debug_hex_show(((LIBGS_BG *)&Bg)[0].r, 0, 16);
}
