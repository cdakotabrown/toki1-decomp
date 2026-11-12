/* bunka_sd_step - BUNKA_SD overlay, Address: 0x80132428, Size: 448 bytes */
/* @SymbolSize: 448 */
/* @RodataBase: 0x80139F5C */
#include "types.h"

extern void update_anim_state(s32);
extern void message_window_show(void);
extern void basyo_show(void);
extern void device_cmd_send(s32);
extern void bunka_sd_init0(void);
extern void sub_sub_step_up(void);
extern void bunka_sd_club0_cd_load(void);
extern void bunka_sd_speaker_set(void);
extern void rollcall_start(void);
extern void bunka_sd_fade_in(void);
extern void bunka_sd_club0_banner_anim_wait(void);
extern void banner_scroll(void);
extern void bunka_sd_daimoku_disp(void);
extern void banner_wait(s32);
extern void bunka_sd_result_step(void);
extern void bunka_sd_rollcall_step(void);
extern void bunka_sd_mainc(void);
extern void k_reset(s32);
extern void bunka_sd_fade_out(void);
extern void bunka_sd_exit(void);
extern void game_reset(void);

extern u8 now_sub_sub_step;
extern s32 step_counter;

void bunka_sd_step(void)
{
    update_anim_state(2);
    message_window_show();
    basyo_show();

    switch ((u32)now_sub_sub_step) {
    case 0:
        device_cmd_send(193);
        bunka_sd_init0();
        break;
    case 1:
        if ((u32)step_counter >= 31U) {
            sub_sub_step_up();
        }
        break;
    case 2:
        bunka_sd_club0_cd_load();
        break;
    case 3:
        bunka_sd_speaker_set();
        break;
    case 4:
        rollcall_start();
        device_cmd_send(228);
        break;
    case 5:
        bunka_sd_fade_in();
        break;
    case 6:
        bunka_sd_club0_banner_anim_wait();
        break;
    case 7:
        banner_scroll();
        bunka_sd_daimoku_disp();
        break;
    case 8:
        banner_wait(15);
        break;
    case 9:
        banner_scroll();
        bunka_sd_result_step();
        break;
    case 10:
        banner_wait(30);
        break;
    case 11:
        banner_scroll();
        bunka_sd_rollcall_step();
        break;
    case 12:
        bunka_sd_mainc();
        break;
    case 13:
        k_reset(1);
        sub_sub_step_up();
        break;
    case 14:
        bunka_sd_fade_out();
        break;
    case 15:
        bunka_sd_exit();
        break;
    default:
        game_reset();
        break;
    }

    step_counter = step_counter + 1;
}
