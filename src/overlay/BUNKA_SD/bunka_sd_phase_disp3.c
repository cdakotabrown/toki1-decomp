/* @RodataBase: 0x8013AA00 */
/* bunka_sd_phase_disp3 - BUNKA_SD overlay, Address: 0x80135B44, Size: 248 bytes (62 insns) */
/* @PostProc: swap_registers v0:t6 */
/* @PostProc: swap_registers t7:t6 */
#include "types.h"

extern void update_anim_state(s32);
extern void message_window_show(void);
extern void basyo_show(void);
extern void bunka_sd_init0(void);
extern void bunka_sd_club2_y97_cd_load(void);
extern void bunka_sd_init2c(void);
extern void bunka_sd_fade_in(void);
extern void bunka_sd_wait4b(void);
extern void device_cmd_send(s32);
extern void k_reset(s32);
extern void sub_sub_step_up(void);
extern void bunka_sd_fade_out(void);
extern void bunka_sd_exit(void);
extern void game_reset(void);

void bunka_sd_phase_disp3(void)
{
    u8 step;

    update_anim_state(2);
    message_window_show();
    basyo_show();

    step = *(u8 *)0x800EF86AU;
    switch (step) {
    case 0:
        bunka_sd_init0();
        break;
    case 1:
        bunka_sd_club2_y97_cd_load();
        break;
    case 2:
        bunka_sd_init2c();
        break;
    case 3:
        bunka_sd_fade_in();
        break;
    case 4:
        bunka_sd_wait4b();
        break;
    case 5:
        device_cmd_send(193);
        k_reset(1);
        sub_sub_step_up();
        break;
    case 6:
        bunka_sd_fade_out();
        break;
    case 7:
        bunka_sd_exit();
        break;
    default:
        game_reset();
        break;
    }
}
