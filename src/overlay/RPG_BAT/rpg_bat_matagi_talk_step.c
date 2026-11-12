/* @SymbolStart: 0x80132FE0 */
/* @SymbolSize: 296 */
/* @RodataBase: 0x8015ABA0 */
#include "types.h"

extern void strcpy(void *, void *);
extern void rpg_bat_msg_set(void *, s32, s32);
extern void rpg_bat_koe_set(s32, s32, s32);
extern void rpg_bat_talk_wait(s32);
extern void rpg_bat_talk_clear(void);
extern void rpg_bat_seq_step_next(void);

extern u8 rpg_bat_msg_buf2[];
extern u8 rpg_bat_msg_buf2_1[];
extern u8 rpg_bat_msg_tbl2[];
extern u8 rpg_bat_matagi_talk_step_str0[];
extern u8 rpg_bat_matagi_talk_step_str1[];
extern s32 rpg_bat_talk_work_step;

void rpg_bat_matagi_talk_step(void)
{
    switch (rpg_bat_talk_work_step) {
    case 0:
        strcpy(rpg_bat_msg_buf2, rpg_bat_matagi_talk_step_str0);
        strcpy(rpg_bat_msg_buf2_1, rpg_bat_matagi_talk_step_str1);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 2, 170);
        rpg_bat_talk_work_step++;
        break;
    case 1:
        rpg_bat_koe_set(0x0B000001, 0x02000008, 0);
        rpg_bat_talk_work_step++;
        break;
    case 2:
        rpg_bat_talk_wait(1);
        break;
    case 3:
        rpg_bat_koe_set(0x1E000047, 0x0F00004B, 1);
        rpg_bat_talk_work_step++;
        break;
    case 4:
        rpg_bat_talk_wait(0);
        break;
    case 5:
        rpg_bat_talk_clear();
        rpg_bat_seq_step_next();
        break;
    }
}
