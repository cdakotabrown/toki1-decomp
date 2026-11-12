/* @CompileFlags: -Wc,-pic1 */
/* @SymbolSize: 1716 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=-30 */
/* @RodataBase: 0x8015DA0C */
/* @PostProc: patch_stack_complete --old 64 --new 72 */
/* @PostProc: swap_instructions --offset1 0x1dc --offset2 0x1e4 */
/* @PostProc: swap_instructions --offset1 0x1d8 --offset2 0x1dc */
/* @PostProc: swap_instructions --offset1 0x54c --offset2 0x550 */
/* @PostProc: patch_insn_reg_field 0x540:rt:v0:a2 0x54c:rt:t6:a1 0x554:rt:t6:a1 0x55c:rt:v0:a2 0x5ec:rt:t7:t6 0x5f0:rs:t7:t6 0x5f0:rt:t7:t6 0x5f8:rs:t7:t6 0x654:rt:t8:t7 0x658:rs:t8:t7 0x658:rt:t8:t7 0x664:rt:t8:t7 0x668:rt:t9:t8 0x66c:rs:t9:t8 0x66c:rt:t9:t8 0x674:rs:t9:t8 0x688:rt:v0:t9 0x68c:rs:v0:t9 0x68c:rt:v0:t9 0x694:rs:v0:t9 0x69c:rs:v0:t9 */
/* utyuuzin_intro - RPG_BAT overlay, Address: 0x80152F30, Size: 1716 bytes */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern s32 sub_step_up();
extern s32 sub_sub_step_up();
extern s32 device_cmd_send();
extern s32 get_CD_State();
extern s32 set_XAsector();
extern s32 dtd_on_tpage();
extern s32 _sprite_set_box_shade();
extern s32 k_disp_switch();
extern s32 strcpy();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 get_rand_mod();
extern s32 rpg_bat_se_set();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_msg_step();
extern s32 himo_utyuuzin_talk();
extern s32 rpg_bat_sprite_set();

extern u8 now_girl;
extern volatile u32 step_counter;
extern volatile u8 now_sub_sub_step;
extern u8 utyuuzin_intro_str0[];
extern u8 rpg_bat_msg_buf4[];
extern u8 rpg_bat_msg_tbl4[];
extern s32 rpg_bat_bg_wave_phase;
extern s32 rpg_bat_bg_wave_amp;
extern s32 rpg_bat_bg_wave_period;
extern s32 rpg_bat_bg_wave_len;
extern s32 rpg_bat_bg_wave_step;
extern s32 rpg_bat_bg_wave_on;
extern s32 rpg_bat_msg_ctl_mode;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit2;

void utyuuzin_intro(void)
{
    u32 color;
    u32 c;
    u32 v1;
    u32 sel;
    u32 v0;
    s32 y;

    switch (get_rand_mod(8)) {
    case 0:
        color = 0x009F9F9F;
        break;
    case 1:
        color = 0x006F6F6F;
        break;
    case 2:
        color = 0x00BF0000;
        break;
    case 3:
        color = 0x0000BF00;
        break;
    case 4:
        color = 0x000000BF;
        break;
    case 5:
        color = 0x00AFAF00;
        break;
    case 6:
        color = 0x00AF00AF;
        break;
    case 7:
        color = 0x0000AFAF;
        break;
    }

    rpg_bat_msg_step();
    c = color;

    switch (now_sub_sub_step) {
    case 0:
        now_girl = 14;
        set_XAsector(0x1C0000A8, 0x0F0000B7, 14);
        device_cmd_send(768);
        sub_sub_step_up();
        break;
    case 1:
        device_cmd_send(228);
        sub_sub_step_up();
        break;
    case 2:
        if (get_CD_State() & 4) {
            sub_sub_step_up();
        }
        break;
    case 3:
        rpg_bat_anim_set2(48, 1, 1, 1);
        rpg_bat_anim_set2(29, 23, 1, 1);
        rpg_bat_sprite_pos_set(48, 352, 88);
        rpg_bat_sprite_pos_set(29, 360, 136);
        rpg_bat_bg_wave_step = 0;
        rpg_bat_bg_wave_len = 0;
        rpg_bat_bg_wave_period = 0;
        rpg_bat_bg_wave_phase = 0;
        y = -30;
        rpg_bat_bg_wave_amp = y;
        rpg_bat_bg_wave_on = 0;
        rpg_bat_sprite_pos_set(47, rpg_bat_bg_wave_phase, y);
        rpg_bat_anim_set2(47, 2, 1, 1);
        rpg_bat_sprite_pos_set(46, rpg_bat_bg_wave_phase, rpg_bat_bg_wave_amp);
        rpg_bat_anim_set2(46, 6, 1, 1);
        sub_sub_step_up();
        break;
    case 4:
        v1 = step_counter;
        rpg_bat_sprite_pos_set(48, 352 - v1, 88);
        v1 = step_counter;
        rpg_bat_sprite_pos_set(29, 360 - v1, 136);
        v1 = step_counter + 4;
        step_counter = v1;
        if (v1 >= 81U) {
            rpg_bat_sprite_pos_set(48, 0, 0);
            rpg_bat_anim_set(48, 0, 1, 1);
            rpg_bat_sprite_pos_set(29, 0, 0);
            rpg_bat_anim_set(29, 24, 1, 1);
            rpg_bat_sprite_pos_set(56, 0, 0);
            rpg_bat_anim_set(56, 9, 1, 1);
            class_w[152].prio = 2;
            sub_sub_step_up();
        }
        break;
    case 5:
        v1 = step_counter;
        v0 = v1 >> 1;
        _sprite_set_box_shade(-92 - v0, -120, v0 << 1, 168, 3, c, 129);
        dtd_on_tpage(0, 0, 3, 1, 0);
        v1 = step_counter + 1;
        step_counter = v1;
        if (v1 >= 65U) {
            sub_sub_step_up();
        }
        break;
    case 6:
        _sprite_set_box_shade(-124, -120, 64, 168, 3, c, 129);
        dtd_on_tpage(0, 0, 3, 1, 0);
        v1 = step_counter;
        rpg_bat_sprite_pos_set(59, 72, v1 - 128);
        rpg_bat_anim_set2(59, 8, 1, 1);
        v1 = step_counter + 1;
        step_counter = v1;
        if (v1 >= 233U) {
            k_disp_switch(rpg_bat_unit1.disp, 1);
            sub_sub_step_up();
        }
        break;
    case 7:
        _sprite_set_box_shade(-124, -120, 64, 168, 3, c, 129);
        v1 = step_counter + 1;
        step_counter = v1;
        dtd_on_tpage(0, 0, 3, 1, 0);
        v1 = step_counter;
        if (v1 >= 121U) {
            sub_sub_step_up();
        }
        break;
    case 8:
        v1 = step_counter;
        _sprite_set_box_shade(v1 - 124, -120, 64 - (v1 << 1), 168, 3, c, 129);
        dtd_on_tpage(0, 0, 3, 1, 0);
        v1 = step_counter + 1;
        step_counter = v1;
        if (v1 >= 33U) {
            sub_sub_step_up();
        }
        break;
    case 9:
        v1 = step_counter;
        v0 = v1 - 64;
        v1 = v1 + 1;
        step_counter = v1;
        rpg_bat_bg_wave_phase = 64;
        rpg_bat_bg_wave_amp = v0;
        if (v1 >= 89U) {
            sub_sub_step_up();
        }
        break;
    case 10:
        strcpy(rpg_bat_msg_buf4, utyuuzin_intro_str0);
        rpg_bat_msg_set(rpg_bat_msg_tbl4, 1, 90);
        rpg_bat_anim_set(56, 255, 1, 0);
        rpg_bat_sprite_pos_set(59, 0, 0);
        rpg_bat_anim_set(59, 0, 1, 1);
        sub_sub_step_up();
        break;
    case 11:
        if (rpg_bat_msg_ctl_mode == 0) {
            sub_sub_step_up();
        }
        break;
    case 12:
        rpg_bat_sprite_pos_set(30, 0, 0);
        rpg_bat_anim_set(30, 0, 5, 1);
        sub_sub_step_up();
        break;
    case 13:
        himo_utyuuzin_talk();
        break;
    case 14:
        rpg_bat_unit2.flag = 0x10001000;
        sub_step_up();
        break;
    }

    if (now_sub_sub_step >= 9U) {
        rpg_bat_sprite_set();
    }
    sel = now_sub_sub_step;
    if (sel >= 5U && sel < 8U) {
        rpg_bat_se_set(1285, 110, 7);
    }
}
