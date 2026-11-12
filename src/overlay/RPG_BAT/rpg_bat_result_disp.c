/* @SymbolSize: 1404 */
/* @PostProc: patch_insn_reg_field 0x484:rt:a1:v0 0x488:rs:a1:v0 0x488:rt:a1:v0 0x490:rd:t4:a1 0x490:rt:a1:v0 0x494:rs:t4:a1 0x4b8:rt:a1:v0 0x4bc:rs:a1:v0 0x4bc:rt:a1:v0 0x4c4:rd:t5:a1 0x4c4:rt:a1:v0 0x4cc:rs:t5:a1 */
/* @RodataBase: 0x8015D9D4 */
/* rpg_bat_result_disp - RPG_BAT overlay, Address: 0x801529B4, Size: 1404 bytes */
#include "types.h"

extern s32 sub_step_up();
extern s32 sub_sub_step_up();
extern s32 device_port_init();
extern s32 strcpy();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_msg_step();

extern u32 step_counter;
extern volatile u8 now_sub_sub_step;
extern u8 rpg_bat_result_disp_str0[];
extern u8 rpg_bat_result_disp_str1[];
extern u8 rpg_bat_result_disp_str2[];
extern u8 rpg_bat_result_disp_str3[];
extern u8 rpg_bat_msg_buf4[];
extern u8 rpg_bat_msg_tbl4[];
extern s32 rpg_bat_mode;
extern s32 rpg_bat_girl;

void rpg_bat_result_disp(void)
{
    u32 v1;
    u32 v0;
    s32 sel;

    rpg_bat_msg_step();

    switch (now_sub_sub_step) {
    case 0:
        device_port_init(1, 512);
        switch (rpg_bat_mode) {
        case 0x10000:
            strcpy(rpg_bat_msg_buf4, rpg_bat_result_disp_str0);
            break;
        case 0x20000:
            strcpy(rpg_bat_msg_buf4, rpg_bat_result_disp_str1);
            break;
        case 0x40000:
            strcpy(rpg_bat_msg_buf4, rpg_bat_result_disp_str2);
            break;
        case 0x100000:
            strcpy(rpg_bat_msg_buf4, rpg_bat_result_disp_str3);
            break;
        }
        if (rpg_bat_mode != 0x8000000) {
            rpg_bat_msg_set(rpg_bat_msg_tbl4, 1, 90);
        }
        rpg_bat_anim_set2(48, 1, 1, 1);
        switch (rpg_bat_mode) {
        case 0x10000:
        case 0x20000:
        case 0x40000:
        case 0x8000000:
            rpg_bat_anim_set2(59, 0, 1, 1);
            break;
        case 0x100000:
            rpg_bat_anim_set2(59, 0, 1, 1);
            rpg_bat_anim_set2(60, 0, 1, 1);
            rpg_bat_anim_set2(61, 0, 1, 1);
            break;
        }
        sel = rpg_bat_girl;
        if (sel != 14) {
            switch (sel) {
            case 0:
                rpg_bat_anim_set2(29, 2, 1, 1);
                break;
            case 1:
                rpg_bat_anim_set2(29, 2, 1, 1);
                break;
            case 2:
                rpg_bat_anim_set2(29, 9, 1, 1);
                break;
            case 3:
                rpg_bat_anim_set2(29, 2, 1, 1);
                break;
            case 4:
                rpg_bat_anim_set2(29, 1, 1, 1);
                break;
            case 5:
                rpg_bat_anim_set2(29, 0, 1, 1);
                break;
            case 6:
                rpg_bat_anim_set2(29, 1, 1, 1);
                break;
            case 7:
                rpg_bat_anim_set2(29, 1, 1, 1);
                break;
            case 8:
                rpg_bat_anim_set2(29, 1, 1, 1);
                break;
            case 9:
                rpg_bat_anim_set2(29, 1, 1, 1);
                break;
            case 10:
                rpg_bat_anim_set2(29, 1, 1, 1);
                break;
            case 13:
                rpg_bat_anim_set2(29, 1, 1, 1);
                break;
            }
        }
        rpg_bat_sprite_pos_set(48, 352, 88);
        rpg_bat_sprite_pos_set(29, 360, 136);
        rpg_bat_sprite_pos_set(59, -88, 96);
        if (rpg_bat_mode == 0x100000) {
            rpg_bat_sprite_pos_set(59, -40, 104);
            rpg_bat_sprite_pos_set(60, -96, 88);
            rpg_bat_sprite_pos_set(61, -104, 136);
        }
        if (rpg_bat_mode == 0x8000000) {
            rpg_bat_sprite_pos_set(59, -88, 112);
        }
        sub_sub_step_up();
        break;
    case 1:
        v1 = step_counter;
        rpg_bat_sprite_pos_set(48, 352 - v1, 88);
        v1 = step_counter;
        rpg_bat_sprite_pos_set(29, 360 - v1, 136);
        v1 = step_counter;
        v0 = v1 << 1;
        rpg_bat_sprite_pos_set(59, v0 - 88, 96);
        if (rpg_bat_mode == 0x100000) {
            v1 = step_counter;
            v0 = v1 << 1;
            rpg_bat_sprite_pos_set(59, v0 - 40, 104);
            v1 = step_counter;
            v0 = v1 << 1;
            rpg_bat_sprite_pos_set(60, v0 - 96, 88);
            v1 = step_counter << 1;
            rpg_bat_sprite_pos_set(61, v1 - 104, 136);
        }
        if (rpg_bat_mode == 0x8000000) {
            v1 = step_counter << 1;
            rpg_bat_sprite_pos_set(59, v1 - 88, 112);
        }
        v1 = step_counter + 4;
        step_counter = v1;
        if (v1 >= 81U) {
            rpg_bat_sprite_pos_set(48, 0, 0);
            rpg_bat_anim_set(48, 0, 1, 1);
            rpg_bat_sprite_pos_set(29, 0, 0);
            if (rpg_bat_girl != 14) {
                rpg_bat_anim_set(29, 0, 1, 1);
            }
            sub_sub_step_up();
        }
        break;
    case 2:
        v1 = step_counter + 1;
        step_counter = v1;
        if (v1 >= 61U) {
            sub_step_up();
        }
        break;
    }
}
