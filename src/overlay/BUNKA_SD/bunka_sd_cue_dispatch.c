/* @RodataBase: 0x8013B494 */
/* @SymbolSize: 544 */
/* bunka_sd_cue_dispatch - BUNKA_SD overlay, Address: 0x80137D6C, Size: 544 bytes (136 insns) */
/* @PostProc: patch_insn_reg_field 0x1c0:rt:v0:t7 0x1c4:rs:v0:t7 0x1c4:rt:v0:t7 0x1cc:rs:v0:t7 0x210:rt:v0:t8 0x214:rs:v0:t8 0x214:rt:v0:t8 0x220:rs:v0:t8 0x220:rt:v0:t9 0x224:rt:v0:t9 */
#include "types.h"

extern volatile s32 step_counter;
extern u8 now_sub_sub_step;

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);
extern void game_reset(void);
extern void k_reset(s32);
extern void message_window_show(void);
extern void basyo_show(void);
extern void update_anim_state(s32);
extern void bunka_sd_init0(void);
extern void bunka_sd_exit(void);
extern void bunka_sd_fade_in(void);
extern void bunka_sd_fade_out(void);
extern void bunka_sd_club1_y96_cd_load(void);
extern void bunka_sd_cue_start(void);
extern void bunka_sd_club1_y96_intro_sprite_set(void);
extern void bunka_sd_club1_y96_cue_sprite_set(void);
extern void bunka_sd_cue_play(void);
extern void bunka_sd_count_wait3(s32);

void bunka_sd_cue_dispatch(void)
{
    u8 step;
    s32 counter;

    update_anim_state(2);
    message_window_show();
    basyo_show();

    step = now_sub_sub_step;
    switch ((u32)step) {
    case 0:
        bunka_sd_init0();
        device_cmd_send(127);
        break;
    case 1:
        bunka_sd_club1_y96_cd_load();
        break;
    case 2:
        device_cmd_send(514);
        bunka_sd_cue_start();
        break;
    case 3:
        bunka_sd_fade_in();
        break;
    case 4:
        bunka_sd_count_wait3(120);
        break;
    case 5:
        bunka_sd_club1_y96_intro_sprite_set();
        break;
    case 6:
        bunka_sd_count_wait3(120);
        break;
    case 7:
        bunka_sd_fade_out();
        break;
    case 8:
        bunka_sd_club1_y96_cue_sprite_set();
        break;
    case 9:
        bunka_sd_fade_in();
        break;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        bunka_sd_cue_play();
        break;
    case 15:
        device_cmd_send(512);
        sub_sub_step_up();
        break;
    case 16:
    case 17:
    case 18:
    case 19:
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
        bunka_sd_cue_play();
        break;
    case 33:
        device_cmd_send(513);
        sub_sub_step_up();
        break;
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
        bunka_sd_cue_play();
        break;
    case 40:
        device_cmd_send(512);
        sub_sub_step_up();
        break;
    case 41:
    case 42:
    case 43:
        bunka_sd_cue_play();
        break;
    case 44:
        device_cmd_send(193);
        device_cmd_send(1280);
        k_reset(1);
        sub_sub_step_up();
        break;
    case 45:
        counter = step_counter;
        if ((u32)counter >= 31U) {
            sub_sub_step_up();
        }
        break;
    case 46:
        bunka_sd_fade_out();
        break;
    case 47:
        bunka_sd_exit();
        break;
    default:
        game_reset();
        break;
    }

    counter = step_counter;
    counter++;
    step_counter = counter;
}
