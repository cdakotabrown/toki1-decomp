/* @RodataBase: 0x8013AD40 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=12:t8,13:t9,14:t0,15:t1,16:t2 IDO52_AS1_XBB_VOLATILE_OK=all */
/* @SymbolSize: 624 */
#include "types.h"

extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern s32 rpg_bat_arg;
extern s32 script_on;
extern s32 date_sprite_on;
extern s32 menu_on;
extern s32 pad_data;
extern volatile u8 cursor_cw_disp;

extern void shugaku_dispatch0(void);
extern void shugaku_dispatch1(void);
extern void shugaku_dispatch2_light(void);
extern void shugaku_dispatch3(void);
extern void shugaku_dispatch4_menu(void);
extern void shugaku_dispatch5(void);
extern void shugaku_dispatch6(void);
extern void shugaku_dispatch7(void);
extern void shugaku_dispatch8_float(void);
extern void shugaku_dispatch9_fade(void);
extern void shugaku_maina(void);
extern void shugaku_mainb(void);

extern void bustup_ani(void);
extern void icon_cursor_move(s32 a0);
extern void update_anim_state(s32 a0);
extern void dec_bg_show(s32 a0);
extern void parameter_show(void);
extern void dir_scroll_main(void);
extern void step_directional_scroll_once(void);
extern void basyo_show(void);
extern void message_window_show(void);
extern void hizuke_show(void);
extern void don_move(void);
extern void game_reset(void);
extern void k_scroll_set(void);
extern void k_disp_wait(void);

void shugaku_main(void)
{
    s32 t4;
    u8 v0v;
    u32 t9v;
    u32 t0v;
    u32 t2v;

    bustup_ani();

    if (now_sub_step == 0) {
        if ((u32)now_sub_sub_step < 17) {
            icon_cursor_move(1);
        }
    }

    switch ((u32)now_sub_step) {
    case 0:  shugaku_dispatch0(); break;
    case 1:  shugaku_dispatch1(); break;
    case 2:  shugaku_dispatch2_light(); break;
    case 3:  shugaku_dispatch3(); break;
    case 4:  shugaku_dispatch4_menu(); break;
    case 5:  shugaku_dispatch5(); break;
    case 6:  shugaku_dispatch6(); break;
    case 7:  shugaku_dispatch7(); break;
    case 8:  shugaku_dispatch8_float(); break;
    case 9:  shugaku_dispatch9_fade(); break;
    case 10: shugaku_maina(); break;
    case 11: shugaku_mainb(); break;
    default: game_reset(); break;
    }

    k_scroll_set();
    k_disp_wait();

    if (rpg_bat_arg == 255) {
        if (now_sub_step == 0 && (u32)now_sub_sub_step < 17) {
            update_anim_state(1);
            dec_bg_show(0);
            parameter_show();
        } else {
            dir_scroll_main();
            update_anim_state(2);
            parameter_show();
        }

        if (script_on == 0 && date_sprite_on == 0 &&
            ((pad_data & (s32)0xF000F000) != 0 || menu_on == 1)) {
            v0v = cursor_cw_disp;
            cursor_cw_disp = (u8)(v0v | 0x80);
        }

        t9v = date_sprite_on;
        v0v = cursor_cw_disp;
        if (t9v != 0) {
            t0v = v0v;
            v0v = t0v & 0x7F;
        }
        t2v = v0v & 0x80;
        cursor_cw_disp = (u8)v0v;
        if (t2v != 0) {
            step_directional_scroll_once();
        }
    }

    basyo_show();
    message_window_show();
    hizuke_show();
    don_move();
}
