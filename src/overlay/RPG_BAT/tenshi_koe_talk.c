/* @SymbolStart: 0x80137AC8 */
/* @SymbolSize: 380 */
/* @RodataBase: 0x8015B7E4 */
#include "types.h"

extern void strcpy(void *, void *);
extern void rpg_bat_msg_set(void *, s32, s32);
extern void rpg_bat_koe_set(s32, s32, s32);
extern void rpg_bat_talk_wait(s32);
extern void rpg_bat_talk_clear(void);
extern void rpg_bat_seq_next(void);

extern u8 rpg_bat_msg_buf4[];
extern u8 rpg_bat_msg4_line1[];
extern u8 rpg_bat_msg4_line2[];
extern u8 rpg_bat_msg_tbl4[];
extern u8 tenshi_koe_talk_str0[];
extern u8 tenshi_koe_talk_str1[];
extern u8 tenshi_koe_talk_str2[];
extern s32 rpg_bat_talk_work_step;

void tenshi_koe_talk(void)
{
    switch (rpg_bat_talk_work_step) {
    case 0:
        strcpy(rpg_bat_msg_buf4, tenshi_koe_talk_str0);
        strcpy(rpg_bat_msg4_line1, tenshi_koe_talk_str1);
        strcpy(rpg_bat_msg4_line2, tenshi_koe_talk_str2);
        rpg_bat_msg_set(rpg_bat_msg_tbl4, 3, 230);
        rpg_bat_talk_work_step++;
        break;
    case 1:
        rpg_bat_koe_set(0x05000013, 0x0300001C, 0);
        rpg_bat_talk_work_step++;
        break;
    case 2:
        rpg_bat_talk_wait(1);
        break;
    case 3:
        rpg_bat_koe_set(0x1F000081, 0x17000086, 1);
        rpg_bat_talk_work_step++;
        break;
    case 4:
        rpg_bat_talk_wait(2);
        break;
    case 5:
        rpg_bat_koe_set(0x0500001D, 0x04000027, 2);
        rpg_bat_talk_work_step++;
        break;
    case 6:
        rpg_bat_talk_wait(0);
        break;
    case 7:
        rpg_bat_talk_clear();
        rpg_bat_seq_next();
        break;
    }
}
