/* @SymbolStart: 0x801535E4 */
/* @RodataBase: 0x8015DA68 */
/* himo_utyuuzin_talk -- RPG_BAT overlay, Address: 0x801535EC, Size: 408 bytes */
/* @SymbolSize: 416 */
#include "types.h"

extern s32 pad_trig;

extern void device_cmd_send(s32);
extern void strcpy(u8 *, u8 *);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_msg_set(u8 *, s32, s32);
extern void rpg_bat_koe_set(s32, s32, s32);
extern void rpg_bat_talk_wait(s32);
extern void rpg_bat_talk_clear(void);
extern void sub_sub_step_up(void);

extern s32 rpg_bat_msg_ctl;
extern s32 rpg_bat_msg_ctl_cnt;
extern s32 rpg_bat_msg_time;
extern s32 rpg_bat_talk_work_step;

extern u8 rpg_bat_msg_buf4[];
extern u8 himo_utyuuzin_talk_str0[];
extern u8 rpg_bat_msg4_line1[];
extern u8 himo_utyuuzin_talk_str1[];
extern u8 rpg_bat_msg_tbl4[];

void himo_utyuuzin_talk(void)
{
    if (pad_trig & 0x40) {
        device_cmd_send(116);
        if (rpg_bat_msg_ctl >= 3 && rpg_bat_msg_ctl_cnt >= 3) {
            rpg_bat_msg_ctl_cnt = rpg_bat_msg_time + 1;
        }
    }

    switch ((u32)rpg_bat_talk_work_step) {
    case 0:
        strcpy(rpg_bat_msg_buf4, himo_utyuuzin_talk_str0);
        strcpy(rpg_bat_msg4_line1, himo_utyuuzin_talk_str1);
        rpg_bat_anim_set(29, 25, 1, 1);
        rpg_bat_msg_set(rpg_bat_msg_tbl4, 2, 150);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 1:
        rpg_bat_koe_set(46, 0x07000033, 0);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 2:
        rpg_bat_talk_wait(1);
        break;
    case 3:
        rpg_bat_koe_set(52, 0x08000037, 1);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 4:
        rpg_bat_talk_wait(0);
        break;
    case 5:
        rpg_bat_anim_set(29, 24, 1, 1);
        rpg_bat_talk_clear();
        sub_sub_step_up();
        break;
    }
}
