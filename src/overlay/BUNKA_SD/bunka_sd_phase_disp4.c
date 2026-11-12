/* @RodataBase: 0x8013AA20 */
/* @PostProc: swap_registers v0:t6 --unidirectional */
/* @PostProc: swap_registers t7:t6 --unidirectional */
/* bunka_sd_phase_disp4 - BUNKA_SD overlay, Address: 0x801363BC, Size: 248 bytes (62 insns) */
#include "types.h"

extern void update_anim_state(s32);
extern void message_window_show(void);
extern void basyo_show(void);
extern void bunka_sd_init0(void);
extern void bunka_sd_club2_y95_cd_load(void);
extern void bunka_sd_init2d(void);
extern void bunka_sd_fade_in(void);
extern void bunka_sd_wait4c(void);
extern void device_cmd_send(s32);
extern void k_reset(s32);
extern void sub_sub_step_up(void);
extern void bunka_sd_fade_out(void);
extern void bunka_sd_exit(void);
extern void game_reset(void);

void bunka_sd_phase_disp4(void)
{
    u8 step;

    update_anim_state(2);
    message_window_show();
    basyo_show();

    step = *(u8 *)0x800EF86AU;
    switch (step) {
    case 0:
        bunka_sd_init0();
        return;
    case 1:
        bunka_sd_club2_y95_cd_load();
        return;
    case 2:
        bunka_sd_init2d();
        return;
    case 3:
        bunka_sd_fade_in();
        return;
    case 4:
        bunka_sd_wait4c();
        return;
    case 5:
        device_cmd_send(193);
        k_reset(1);
        sub_sub_step_up();
        return;
    case 6:
        bunka_sd_fade_out();
        return;
    case 7:
        bunka_sd_exit();
        return;
    default:
        game_reset();
        break;
    }
}
