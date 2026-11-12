/* @RodataBase: 0x80139700 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=1 IDO52_UGEN_TEMP_REG=9:t5,10:t6,11:t7,12:t8,13:t9,14:t0,15:t1,16:t3,17:t4,18:t5,20:t7 */
/* Address: 0x80133008, Size: 1448 bytes (362 insns) */
/* `@RodataBase: 0x80139700` = the 33-entry jr table (the only unresolved */
#include "types.h"

extern void bustup_ani(void);
extern void icon_cursor_move(s32);
extern void kangei_scene_init(void);
extern void kangei_load_wait_step(void);
extern void kangei_intro_dispatch(void);
extern void vacation_day_init(void);
extern void kangei_bukatu_step(void);
extern void kangei_slot_match(void);
extern void kangei_schedule_next(void);
extern void kangei_girl_event_check(void);
extern void kangei_frame_check(void);
extern void kangei_main1d(void);
extern void kangei_main1f(void);
extern void kangei_scene_exit(void);
extern void game_reset(void);
extern void parameter_show(void);
extern void k_scroll_set(void);
extern void k_disp_wait(void);
extern void basyo_show(void);
extern void message_window_show(void);
extern void hizuke_show(void);
extern void don_move(void);
extern void update_anim_state(s32);
extern void dir_scroll_main(void);
extern void dec_bg_show(s32 a0);
extern void step_directional_scroll_once(void);

extern volatile u8  now_sub_step;
extern volatile u8  now_sub_sub_step;
extern u8  kangei_icon_flag;
extern volatile s32 script_on;
extern s32  date_sprite_on;
extern volatile s32 pad_data;
extern volatile s32 menu_on;
extern volatile u8  cursor_cw_disp;

void kangei_scene_main(void)
{
    u32 state;
    u32 sub_state;
    u32 flag;
    s32 input;
    u8 val;

    bustup_ani();

    state = now_sub_step;

    if (state == 2 || state == 5 || state == 9 || state == 10 ||
        state == 12 || state == 15 || state == 17 || state == 20 ||
        state == 22 || state == 25 || state == 29 || state == 31) {
        if (now_sub_sub_step != 0) {
            flag = kangei_icon_flag;
            if (flag == 0) {
                goto skip_unlock;
            }
        }
    }

    flag = kangei_icon_flag;
    if (state == 30 && flag == 0) {
        goto skip_unlock;
    }

    icon_cursor_move(1);

    state = now_sub_step;

skip_unlock:

    switch (state) {
    case 0 : kangei_scene_init(); break;
    case 1 : kangei_load_wait_step(); break;
    case 2 : kangei_intro_dispatch(); break;
    case 3 : vacation_day_init(); break;
    case 4 : kangei_bukatu_step(); break;
    case 5 : kangei_slot_match(); break;
    case 6 : kangei_schedule_next(); break;
    case 7 : vacation_day_init(); break;
    case 8 : kangei_bukatu_step(); break;
    case 9 : kangei_girl_event_check(); break;
    case 10: kangei_slot_match(); break;
    case 11: kangei_schedule_next(); break;
    case 12: kangei_frame_check(); break;
    case 13: vacation_day_init(); break;
    case 14: kangei_bukatu_step(); break;
    case 15: kangei_slot_match(); break;
    case 16: kangei_schedule_next(); break;
    case 17: kangei_frame_check(); break;
    case 18: vacation_day_init(); break;
    case 19: kangei_bukatu_step(); break;
    case 20: kangei_slot_match(); break;
    case 21: kangei_schedule_next(); break;
    case 22: kangei_frame_check(); break;
    case 23: vacation_day_init(); break;
    case 24: kangei_bukatu_step(); break;
    case 25: kangei_slot_match(); break;
    case 26: kangei_schedule_next(); break;
    case 27: vacation_day_init(); break;
    case 28: kangei_bukatu_step(); break;
    case 29: kangei_main1d(); break;
    case 30: kangei_slot_match(); break;
    case 31: kangei_main1f(); break;
    case 32: kangei_scene_exit(); break;
    default: game_reset(); break;
    }

    state = now_sub_step;

    if (state == 3 || state == 7 || state == 13 || state == 18 ||
        state == 23 || state == 27) {
        icon_cursor_move(1);
    }

    parameter_show();
    k_scroll_set();
    k_disp_wait();
    basyo_show();
    message_window_show();
    hizuke_show();
    don_move();

    state = now_sub_step;

    if (state == 2 || state == 5 || state == 9 || state == 10 ||
        state == 12 || state == 15 || state == 17 || state == 20 ||
        state == 22 || state == 25 || state == 29 || state == 31) {
        if (now_sub_sub_step != 0) {
            flag = kangei_icon_flag;
            if (flag == 0) {
                update_anim_state(2);
                dir_scroll_main();
                goto tail;
            }
        }
    }

    flag = kangei_icon_flag;
    if (state == 30 && flag == 0) {
        update_anim_state(2);
        dir_scroll_main();
        goto tail;
    }

    if (flag == 1) {
        dec_bg_show(0);

        if (script_on == 0 && date_sprite_on == 0) {

            if ((pad_data & 0xF000F000) != 0 || menu_on == 1) {
                val = cursor_cw_disp;
                cursor_cw_disp = val | 0x80;
            }
        }

        val = cursor_cw_disp;
        if (val & 0x80) {
            step_directional_scroll_once();
        }

        update_anim_state(1);
        goto tail;
    }

    dec_bg_show(0);
    update_anim_state(1);

    if (script_on == 0 && date_sprite_on == 0) {
        if ((pad_data & 0xF000F000) != 0 || menu_on == 1) {
            val = cursor_cw_disp;
            cursor_cw_disp = val | 0x80;
        }
    }

    val = cursor_cw_disp;
    if (date_sprite_on != 0) {
        val = val & 0x7F;
    }
    cursor_cw_disp = val;

    if (val & 0x80) {
        step_directional_scroll_once();
    }

tail:
    don_move();
}
