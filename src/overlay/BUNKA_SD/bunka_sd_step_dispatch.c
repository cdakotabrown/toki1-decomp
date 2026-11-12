/* @RodataBase: 0x8013A904 */
/* bunka_sd_step_dispatch - BUNKA_SD overlay, Address: 0x8013376C, Size: 440 bytes (110 insns) */
/* @SymbolSize: 440 */
#include "types.h"

extern void update_anim_state(s32);
extern void message_window_show(void);
extern void basyo_show(void);
extern void device_cmd_send(s32);
extern void bunka_sd_init0(void);
extern void bunka_sd_club1_y95_cd_load(void);
extern void result_start(void);
extern void bunka_sd_fade_in(void);
extern void bunka_sd_rec_play2(void);
extern void bunka_sd_fade_out(void);
extern void bunka_sd_sprite_flag_set(void);
extern void sub_sub_step_up(void);
extern void k_reset(s32);
extern void bunka_sd_exit(void);
extern void game_reset(void);

extern u8 now_sub_sub_step;
extern s32 step_counter;

void bunka_sd_step_dispatch(void)
{
    u8 step;
    s32 counter;

    update_anim_state(2);
    message_window_show();
    basyo_show();

    step = now_sub_sub_step;
    switch (step) {
    case 0:
        bunka_sd_init0();
        device_cmd_send(127);
        break;
    case 1:
        bunka_sd_club1_y95_cd_load();
        break;
    case 2:
        device_cmd_send(512);
        result_start();
        break;
    case 3:
        bunka_sd_fade_in();
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        bunka_sd_rec_play2();
        break;
    case 12:
        bunka_sd_fade_out();
        break;
    case 13:
        device_cmd_send(513);
        bunka_sd_sprite_flag_set();
        break;
    case 14:
        bunka_sd_fade_in();
        break;
    case 15:
    case 16:
    case 17:
    case 18:
        bunka_sd_rec_play2();
        break;
    case 19:
        device_cmd_send(514);
        sub_sub_step_up();
        break;
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
        bunka_sd_rec_play2();
        break;
    case 37:
        device_cmd_send(193);
        device_cmd_send(1280);
        k_reset(1);
        sub_sub_step_up();
        break;
    case 38:
        counter = step_counter;
        if ((u32)counter >= 31U) {
            sub_sub_step_up();
        }
        break;
    case 39:
        bunka_sd_fade_out();
        break;
    case 40:
        bunka_sd_exit();
        break;
    default:
        game_reset();
    }

    counter = step_counter;
    counter++;
    step_counter = counter;
}
