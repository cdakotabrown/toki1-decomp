/* @SymbolSize: 744 */
/* rpg_bat_frame - RPG_BAT overlay, Address: 0x8013B910, Size: 744 bytes (186 insns) */
/* 2026-07-30 also removed `@PostProc: scheduler_normalizer` (whole-function). */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 device_cmd_send();
extern s32 k_disp_switch();
extern s32 rpg_bat_ready_set();
extern s32 rpg_bat_win_seq_sel();
extern s32 rpg_bat_wave_disp();
extern s32 rpg_bat_enemy_main();
extern s32 rpg_bat_phase_run();
extern s32 rpg_bat();
extern s32 rpg_bat_counter_disp();
extern s32 rpg_bat_msg_step();
extern s32 rpg_bat_slot_step0();
extern s32 rpg_bat_slot_step1();
extern s32 rpg_bat_slot_step2();
extern s32 rpg_bat_slot_step3();
extern s32 rpg_bat_sprite_set();

extern s32 pad_trig;
extern s32 rpg_bat_msg_time;
extern s32 rpg_bat_mode;
extern s32 rpg_bat_pause_mode;
extern s32 rpg_bat_pause_cnt;
extern s32 rpg_bat_msg_ctl;
extern s32 rpg_bat_msg_ctl_cnt;
extern s32 rpg_bat_menu_ctl;
extern s32 rpg_bat_slot0_ctl_mode;
extern s32 rpg_bat_slot1_ctl_mode;
extern s32 rpg_bat_slot2_ctl_mode;
extern s32 rpg_bat_slot3_ctl_mode;
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit2;
extern RPG_BAT_UNIT rpg_bat_unit3;

s32 rpg_bat_frame(void)
{
    s32 a0;
    s32 a1;
    s32 v0;
    s32 v1;

    rpg_bat_wave_disp();
    if (rpg_bat_pause_mode == 3) {
        rpg_bat_unit2.aux08 = rpg_bat_unit2.aux08 + 1;
        rpg_bat();
        rpg_bat_msg_step();
        return 0;
    }

    if ((pad_trig & 0x40) != 0) {
        if (rpg_bat_pause_cnt == 0) {
            device_cmd_send(116);
            if (rpg_bat_msg_ctl >= 3) {
                if (rpg_bat_msg_ctl_cnt >= 3) {
                    rpg_bat_msg_ctl_cnt = rpg_bat_msg_time + 1;
                }
            }
        }
    }

    v1 = rpg_bat_unit1.flag;
    if (v1 != 0x08000002 && (v1 << 4) < 0 && rpg_bat_unit3.flag != 1) {
        v0 = rpg_bat_menu_ctl;
        if (v0 == 2 || v0 == 3) {
            k_disp_switch(rpg_bat_unit3.disp, 1);
            k_disp_switch(rpg_bat_unit1.disp, 0);
            v1 = rpg_bat_unit1.flag;
        } else {
            k_disp_switch(rpg_bat_unit3.disp, 0);
            k_disp_switch(rpg_bat_unit1.disp, 1);
            v1 = rpg_bat_unit1.flag;
        }
    }

    a0 = rpg_bat_unit0.flag;
    v0 = v1 & 0xFF;
    if ((a0 & 0xFF0111FF) == 0) {
        rpg_bat_unit0.aux08 = rpg_bat_unit0.aux08 + 1;
    }

    if (v0 == 0 && (a0 & 0xFF) == 0) {
        a1 = rpg_bat_unit2.flag;
        if ((a1 & 0xFF) == 0 && (v1 & 0x1000) == 0) {
            rpg_bat_unit1.aux08 = rpg_bat_unit1.aux08 + 1;
        }
    }

    a1 = rpg_bat_unit2.flag;
    if (v0 == 0 && (a0 & 0xFF) == 0 && (a1 & 0xFF) == 0 &&
        (a1 & 0x1000) == 0 && (a1 & 0x80000000) == 0) {
        rpg_bat_unit2.aux08 = rpg_bat_unit2.aux08 + 1;
    }

    rpg_bat();
    rpg_bat_ready_set();
    rpg_bat_phase_run();
    rpg_bat_ready_set();
    rpg_bat_enemy_main();
    rpg_bat_ready_set();
    rpg_bat_win_seq_sel();
    rpg_bat_counter_disp();
    rpg_bat_msg_step();

    if (rpg_bat_slot0_ctl_mode == 1) {
        rpg_bat_slot_step0();
    }
    if (rpg_bat_slot1_ctl_mode == 1) {
        rpg_bat_slot_step1();
    }
    if (rpg_bat_slot2_ctl_mode == 1) {
        rpg_bat_slot_step2();
    }
    if (rpg_bat_slot3_ctl_mode == 1) {
        rpg_bat_slot_step3();
    }
    if (rpg_bat_mode == 0x80000000) {
        rpg_bat_sprite_set();
    }

}
