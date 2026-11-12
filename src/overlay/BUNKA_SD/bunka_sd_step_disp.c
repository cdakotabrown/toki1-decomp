/* @RodataBase: 0x8013AD04 */
/* bunka_sd_step_disp - BUNKA_SD overlay, Address: 0x80136BFC, Size: 736 bytes (184 insns) */
/* @SymbolSize: 736 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);
extern void game_reset(void);
extern void basyo_show(void);
extern void update_anim_state(s32);
extern void message_window_show(void);
extern void k_reset(s32);
extern void bunka_sd_init0(void);
extern void bunka_sd_fade_in(void);
extern void bunka_sd_fade_out(void);
extern void bunka_sd_exit(void);
extern void bunka_sd_club1_y97_cd_load(void);
extern void bunka_sd_rec_start(void);
extern void bunka_sd_sprite_set6(void);
extern void bunka_sd_sprite_set1a(void);
extern void bunka_sd_sprite_set10(void);
extern void bunka_sd_rec_play(void);
extern void bunka_sd_page_next(void);
extern void bunka_sd_count_wait2(s32);
extern void bunka_sd_count_wait3(s32);

extern u8 now_sub_sub_step;
extern s32 step_counter;

void bunka_sd_step_disp(void)
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
        bunka_sd_club1_y97_cd_load();
        break;
    case 2:
        device_cmd_send(512);
        bunka_sd_rec_start();
        break;
    case 3:
        bunka_sd_fade_in();
        break;
    case 4:
        bunka_sd_count_wait3(120);
        break;
    case 5:
        bunka_sd_fade_out();
        break;
    case 6:
        bunka_sd_sprite_set6();
        break;
    case 7:
        bunka_sd_fade_in();
        break;
    case 8:
    case 9:
    case 10:
    case 11:
        bunka_sd_rec_play();
        break;
    case 12:
        device_cmd_send(193);
        sub_sub_step_up();
        break;
    case 13:
        bunka_sd_fade_out();
        break;
    case 14:
        bunka_sd_page_next();
        break;
    case 15:
        bunka_sd_count_wait2(60);
        break;
    case 16:
        bunka_sd_sprite_set10();
        break;
    case 17:
        bunka_sd_count_wait2(60);
        break;
    case 18:
        device_cmd_send(513);
        sub_sub_step_up();
        break;
    case 19:
        bunka_sd_fade_in();
        break;
    case 20:
        bunka_sd_count_wait2(120);
        break;
    case 21:
        bunka_sd_fade_out();
        break;
    case 22:
        bunka_sd_count_wait2(30);
        break;
    case 23:
        bunka_sd_page_next();
        break;
    case 24:
        bunka_sd_count_wait2(15);
        break;
    case 25:
        bunka_sd_page_next();
        break;
    case 26:
        bunka_sd_sprite_set1a();
        break;
    case 27:
        bunka_sd_fade_in();
        break;
    case 28:
    case 29:
    case 30:
        bunka_sd_rec_play();
        break;
    case 31:
        device_cmd_send(514);
        sub_sub_step_up();
        break;
    case 32:
        bunka_sd_count_wait2(120);
        break;
    case 33:
    case 34:
        bunka_sd_rec_play();
        break;
    case 35:
        device_cmd_send(193);
        device_cmd_send(1280);
        k_reset(1);
        sub_sub_step_up();
        break;
    case 36:
        counter = step_counter;
        if ((u32)counter >= 31U) {
            sub_sub_step_up();
        }
        break;
    case 37:
        bunka_sd_fade_out();
        break;
    case 38:
        bunka_sd_exit();
        break;
    default:
        game_reset();
    }

    counter = step_counter;
    counter++;
    step_counter = counter;
}
