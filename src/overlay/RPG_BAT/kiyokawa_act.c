/* @SymbolStart: 0x801568A4 */
/* @RodataBase: 0x8015DCB0 */
/* @SymbolSize: 1048 */
/* @PostProc: patch_insn_reg_field 0x188:rt:v0:v1 0x18c:rs:v0:v1 0x18c:rt:v0:v1 0x194:rs:v0:v1 0x194:rt:v0:v1 0x198:rt:v0:v1 0x19c:rs:v0:v1 0x340:rt:v0:v1 0x344:rs:v0:v1 0x344:rt:v0:v1 0x34c:rs:v0:v1 0x34c:rt:v0:v1 0x350:rt:v0:v1 0x354:rs:v0:v1 */
/* kiyokawa_act - RPG_BAT overlay, Address: 0x801568AC, Size: 1040 bytes (260 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern u8 kiyokawa_act_str0[];
extern u8 kiyokawa_act_str1[];
extern u8 kiyokawa_act_str2[];
extern u8 kiyokawa_act_str3[];
extern u8 rpg_bat_msg_buf3[];
extern u8 rpg_bat_msg_buf3_1[];
extern u8 rpg_bat_msg_tbl3[];
extern s32 pad_trig;
extern s32 rpg_bat_msg_time;
extern s32 rpg_bat_msg_ctl_mode;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit2;

extern s32 get_CD_State();
extern void strcpy();
extern void rpg_bat_anim_set();
extern s32 rpg_bat_rec_flag_check();
extern void rpg_bat_se_set();
extern void rpg_bat_koe_set();
extern void rpg_bat_slide_out();
extern void rpg_bat_slide_in();
extern void rpg_bat_msg_set();
extern void rpg_bat_act_next();
extern void rpg_bat_reset();
extern void rpg_bat_act_wait();
extern void rpg_bat_act_gate();
extern s32 bat_zyotai_check();

void kiyokawa_act(void)
{
    s32 temp;

    switch ((u32)rpg_bat_unit2.step) {
    case 0:
        if (bat_zyotai_check() != 0) {
            strcpy(rpg_bat_msg_buf3, kiyokawa_act_str0);
            strcpy(rpg_bat_msg_buf3_1, kiyokawa_act_str1);
        } else {
            strcpy(rpg_bat_msg_buf3, kiyokawa_act_str2);
            strcpy(rpg_bat_msg_buf3_1, kiyokawa_act_str3);
        }
        rpg_bat_act_next();
        break;
    case 1:
        rpg_bat_slide_out(1, 0);
        break;
    case 2:
        rpg_bat_anim_set(29, 5, 5, 1);
        rpg_bat_se_set(1297, 1, 0);
        rpg_bat_act_next();
        break;
    case 3:
        if (class_w[125].frame == 4) {
            rpg_bat_msg_set(rpg_bat_msg_tbl3, 2, 80);
            if (bat_zyotai_check() != 0) {
                rpg_bat_koe_set(0x1F00005A, 0x1000005E, 0);
            } else {
                rpg_bat_koe_set(0x1F00005F, 0x11000063, 0);
            }
            rpg_bat_act_next();
        }
        break;
    case 4:
        rpg_bat_act_gate(1);
        break;
    case 5:
        temp = rpg_bat_unit2.cnt + 1;
        rpg_bat_unit2.cnt = temp;
        if (temp >= 301) {
            rpg_bat_act_next();
        }
        if (get_CD_State() == 2) {
            rpg_bat_act_next();
        }
        break;
    case 6:
        if (bat_zyotai_check() != 0) {
            rpg_bat_msg_ctl_mode = 3;
            rpg_bat_koe_set(0x03000011, 0x0300001C, 1);
            rpg_bat_anim_set(29, 7, 5, 1);
        } else {
            rpg_bat_koe_set(0x1F000064, 0x12000068, 1);
            class_w[125].rate = 130;
            rpg_bat_anim_set(29, 6, 5, 1);
        }
        rpg_bat_act_next();
        break;
    case 7:
        rpg_bat_msg_time = 90;
        if (rpg_bat_rec_flag_check(29) != 0) {
            if (bat_zyotai_check() != 0) {
                rpg_bat_act_next();
            } else {
                rpg_bat_unit2.step = 20;
            }
        }
        break;
    case 8:
        rpg_bat_anim_set(29, 7, 5, 1);
        rpg_bat_act_next();
        break;
    case 9:
        if ((pad_trig & 0x40) != 0) {
            rpg_bat_anim_set(29, 7, 5, 1);
            if (rpg_bat_msg_ctl_mode == 3) {
                rpg_bat_msg_ctl_mode = 1;
            }
            rpg_bat_unit2.step = 20;
        }
        if (rpg_bat_rec_flag_check(29) != 0) {
            rpg_bat_anim_set(29, 7, 5, 1);
            temp = rpg_bat_unit2.cnt + 1;
            rpg_bat_unit2.cnt = temp;
            if (temp >= 5) {
                if (rpg_bat_msg_ctl_mode == 3) {
                    rpg_bat_msg_ctl_mode = 1;
                }
                rpg_bat_unit2.step = 20;
            }
        }
        break;
    case 20:
        rpg_bat_act_wait(40);
        break;
    case 21:
        rpg_bat_anim_set(29, 0, 1, 1);
        rpg_bat_act_next();
        break;
    case 22:
        class_w[125].rate = 1;
        rpg_bat_anim_set(29, 3, 1, 1);
        rpg_bat_act_next();
        break;
    case 23:
        rpg_bat_slide_in(3, 0);
        break;
    case 24:
        rpg_bat_act_wait(20);
        break;
    case 25:
        rpg_bat_reset();
        break;
    }
}
