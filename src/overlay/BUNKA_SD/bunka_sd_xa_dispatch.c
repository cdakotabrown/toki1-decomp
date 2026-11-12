/* @RodataBase: 0x8013B560 */
/* bunka_sd_xa_dispatch - BUNKA_SD overlay, Address: 0x80138B3C, Size: 256 bytes (64 insns) */
/* @PostProc: swap_registers v0:t6 */
/* @PostProc: swap_registers t7:t6 */
#include "types.h"

extern void update_anim_state(s32);
extern void message_window_show(void);
extern void basyo_show(void);
extern void bunka_sd_init0(void);
extern void device_cmd_send(s32);
extern void bunka_sd_club5_cd_load(void);
extern void bunka_sd_club5_screen_init(void);
extern void bunka_sd_fade_in(void);
extern void bunka_sd_club5_pose_wait(void);
extern void k_reset(s32);
extern void sub_sub_step_up(void);
extern void bunka_sd_fade_out3(void);
extern void bunka_sd_exit2(void);
extern void game_reset(void);

void bunka_sd_xa_dispatch(void)
{
    u8 step;

    update_anim_state(2);
    message_window_show();
    basyo_show();

    step = *(u8 *)0x800EF86AU;
    switch (step) {
    case 0:
        bunka_sd_init0();
        device_cmd_send(127);
        break;
    case 1:
        bunka_sd_club5_cd_load();
        break;
    case 2:
        bunka_sd_club5_screen_init();
        device_cmd_send(228);
        break;
    case 3:
        bunka_sd_fade_in();
        break;
    case 4:
        bunka_sd_club5_pose_wait();
        break;
    case 5:
        k_reset(1);
        sub_sub_step_up();
        break;
    case 6:
        bunka_sd_fade_out3();
        break;
    case 7:
        bunka_sd_exit2();
        break;
    default:
        game_reset();
        break;
    }
}
