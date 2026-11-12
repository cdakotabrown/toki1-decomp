/* @RodataBase: 0x8013A9D0 */
/* bunka_sd_phase_disp2 - BUNKA_SD overlay, Address: 0x801352F4, Size: 280 bytes (70 insns) */
/* @PostProc: swap_registers v0:t6 */
/* @PostProc: swap_registers t7:t6 */
#include "types.h"

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
extern void bunka_sd_club2_y96_cd_load(void);
extern void bunka_sd_init2b(void);
extern void bunka_sd_wait4d(void);
extern void bunka_sd_slot_ani(void);
extern void bunka_sd_wait5(void);

void bunka_sd_phase_disp2(void)
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
        bunka_sd_club2_y96_cd_load();
        break;
    case 2:
        bunka_sd_init2b();
        break;
    case 3:
        bunka_sd_fade_in();
        break;
    case 4:
        bunka_sd_wait4d();
        break;
    case 5:
        bunka_sd_wait5();
        break;
    case 6:
        bunka_sd_slot_ani();
        break;
    case 7:
        device_cmd_send(193);
        k_reset(1);
        sub_sub_step_up();
        break;
    case 8:
        bunka_sd_fade_out();
        break;
    case 9:
        bunka_sd_exit();
        break;
    default:
        game_reset();
        break;
    }
}
