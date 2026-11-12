/* @SymbolStart: 0x801563A0 */
/* @SymbolSize: 336 */
/* @RodataBase: 0x8015DC00 */
#include "types.h"

extern void strcpy(void *, void *);
extern void rpg_bat_msg_set(void *, s32, s32);
extern void rpg_bat_koe_set(s32, s32, s32);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_talk_wait(s32);
extern void rpg_bat_talk_clear(void);
extern void rpg_bat_act_next(void);

extern u8 rpg_bat_msg_buf3[];
extern u8 rpg_bat_msg_buf3_1[];
extern u8 rpg_bat_msg_tbl3[];
extern u8 asahina_talk_str0[];
extern u8 asahina_talk_str1[];
extern s32 rpg_bat_talk_work_step;

void asahina_talk(void)
{
    switch (rpg_bat_talk_work_step) {
    case 0:
        strcpy(rpg_bat_msg_buf3, asahina_talk_str0);
        strcpy(rpg_bat_msg_buf3_1, asahina_talk_str1);
        rpg_bat_msg_set(rpg_bat_msg_tbl3, 2, 160);
        rpg_bat_talk_work_step++;
        break;
    case 1:
        rpg_bat_koe_set(0x1F000015, 0x05000017, 0);
        rpg_bat_anim_set(29, 7, 1, 1);
        rpg_bat_talk_work_step++;
        break;
    case 2:
        rpg_bat_talk_wait(1);
        break;
    case 3:
        rpg_bat_koe_set(0x1F000018, 0x0600001D, 1);
        rpg_bat_anim_set(29, 8, 1, 1);
        rpg_bat_talk_work_step++;
        break;
    case 4:
        rpg_bat_talk_wait(2);
        break;
    case 5:
        rpg_bat_talk_clear();
        rpg_bat_act_next();
        break;
    }
}
