/* @SymbolStart: 0x80146AB4 */
/* @RodataBase: 0x8015C948 */
/* @SymbolSize: 380 */
#include "types.h"

extern s32 strcpy(s32 a0, s32 a1);
extern s32 rpg_bat_koe_set(s32 a0, s32 a1, s32 a2);
extern s32 rpg_bat_talk_clear(void);
extern s32 rpg_bat_talk_wait(s32 a0);
extern s32 rpg_bat_msg_set(s32 a0, s32 a1, s32 a2);
extern s32 rpg_bat_seq_step_next(void);

extern s32 rpg_bat_talk_work_step;
extern u8 rpg_bat_talk3_str0[];
extern u8 rpg_bat_talk3_str1[];
extern u8 rpg_bat_talk3_str2[];
extern u8 rpg_bat_msg_buf2[];
extern u8 rpg_bat_msg_buf2_1[];
extern u8 rpg_bat_msg_buf2_2[];
extern u8 rpg_bat_msg_tbl2[];

void komusou_leave_talk(void) {
    switch (rpg_bat_talk_work_step) {
    case 0:
        strcpy((s32)rpg_bat_msg_buf2, (s32)rpg_bat_talk3_str0);
        strcpy((s32)rpg_bat_msg_buf2_1, (s32)rpg_bat_talk3_str1);
        strcpy((s32)rpg_bat_msg_buf2_2, (s32)rpg_bat_talk3_str2);
        rpg_bat_msg_set((s32)rpg_bat_msg_tbl2, 3, 200);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 1:
        rpg_bat_koe_set(0x1D0000AA, 0x1F0000AE, 0);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 2:
        rpg_bat_talk_wait(1);
        break;
    case 3:
        rpg_bat_koe_set(0x0D0000B2, 0x0D0000B7, 1);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 4:
        rpg_bat_talk_wait(2);
        break;
    case 5:
        rpg_bat_koe_set(0x1000008A, 0x05000092, 2);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 6:
        rpg_bat_talk_wait(0);
        break;
    case 7:
        rpg_bat_talk_clear();
        rpg_bat_seq_step_next();
        break;
    }
}
