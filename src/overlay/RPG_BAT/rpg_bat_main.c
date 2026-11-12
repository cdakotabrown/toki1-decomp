/* @SymbolStart: 0x8013B5B0 */
/* @RodataBase: 0x8015BBA0 */
/* @SymbolSize: 864 */
/* @PostProc: patch_insn_reg_field 0x2f4:rt:t7:v1 0x300:rt:t7:v1 0x330:rt:t8:t7 0x334:rs:t8:t7 0x334:rt:t8:t7 0x33c:rs:t8:t7 0x33c:rt:t9:t8 0x340:rs:t9:t8 */
/* rpg_bat_main - RPG_BAT overlay, Address: 0x8013B5B8, Size: 856 bytes (214 insns) */
#include "types.h"

extern s32 sub_step_up();
extern s32 back_clear_switch();
extern s32 device_cmd_send();
extern s32 device_state_init();
extern s32 device_load_poll();
extern s32 device_port_init();
extern s32 CDQ_init();
extern s32 season_bgm_set();
extern s32 step_directional_scroll_once();
extern s32 set_flag_b3_high_bit();
extern s32 _sprite_set_box_shade();
extern s32 rpg_bat_frame();
extern s32 rpg_bat_pause();
extern s32 rpg_bat_wave_disp();
extern s32 rpg_bat_xa_set();
extern s32 rpg_bat_sd_load();
extern s32 rpg_bat_cd_read();
extern s32 rpg_bat_rec_init();
extern s32 rpg_bat_coord_init();
extern s32 rpg_bat_palette_load();
extern s32 rpg_bat_para_init();
extern s32 rpg_bat_counter_disp();
extern s32 rpg_bat_msg_step();
extern s32 rpg_bat_ptr_set();
extern s32 rpg_bat_init();
extern s32 rpg_bat_exit();
extern s32 rpg_bat_disp_init();
extern s32 rpg_bat_fade_out();
extern s32 rpg_bat_result_disp();
extern s32 utyuuzin_intro();
extern s32 rpg_bat_bg_scroll();

extern u8 menu_mode;
extern s32 step_state;
extern u8 now_sub_step;
extern s32 rpg_bat_mode;

void rpg_bat_main(void)
{
    s32 sel = now_sub_step;

    s32 v0;

    if ((u32)sel < 6) {
        goto check9;
    }
    if ((u32)sel >= 10) {
        goto check9;
    }
    rpg_bat_pause();
    sel = now_sub_step;

check9:
    if (sel == 9) {
        goto check5;
    }
    if ((menu_mode & 1) != 0) {
        goto check5;
    }
    step_directional_scroll_once();
    sel = now_sub_step;

check5:
    if ((u32)sel < 5) {
        goto dispatch;
    }
    if ((u32)sel < 12) {
        _sprite_set_box_shade(-160, 48, 8, 72, 2, 0, 1);
        _sprite_set_box_shade(-160, 112, 320, 8, 2, 0, 1);
        _sprite_set_box_shade(152, 48, 8, 72, 2, 0, 1);
        sel = now_sub_step;
    }

dispatch:
    switch ((u32)sel) {
    case 0:
        rpg_bat_init();
        rpg_bat_para_init();
        break;
    case 1:
        rpg_bat_sd_load();
        break;
    case 2:
        rpg_bat_cd_read();
        break;
    case 3:
        rpg_bat_ptr_set();
        rpg_bat_palette_load();
        break;
    case 4:
        rpg_bat_rec_init();
        rpg_bat_coord_init();
        rpg_bat_disp_init();
        rpg_bat_counter_disp();
        sub_step_up();
        break;
    case 5:
        rpg_bat_bg_scroll();
        break;
    case 6:
        back_clear_switch(0);
        rpg_bat_counter_disp();
        if (rpg_bat_mode == 0x80000000) {
            utyuuzin_intro();
        } else {
            rpg_bat_result_disp();
        }
        break;
    case 7:
        rpg_bat_msg_step();
        rpg_bat_xa_set();
        break;
    case 8:
        if (rpg_bat_mode == 0x80000000) {
            device_port_init(1, 512);
        }
        sub_step_up();
        break;
    case 9:
        rpg_bat_frame();
        break;
    case 10:
        device_cmd_send(193);
        sub_step_up();
        break;
    case 11:
        rpg_bat_wave_disp();
        rpg_bat_fade_out();
        break;
    case 12:
        device_state_init();
        CDQ_init();
        sub_step_up();
        break;
    case 13:
        season_bgm_set();
        sub_step_up();
        break;
    case 14:
        v0 = step_state + 1;
        step_state = v0;
        if ((u32)v0 < 1025) {
            goto poll450BC;
        }
        device_state_init();
        CDQ_init();
        now_sub_step = 13;
        break;
    poll450BC:
        if (device_load_poll() != 1) {
            break;
        }
        sub_step_up();
        break;
    case 15:
    default:
        rpg_bat_exit();
        break;
    }

    if ((menu_mode & 1) == 0) {
        set_flag_b3_high_bit(1);
        return;
    }
    set_flag_b3_high_bit(0);
}
