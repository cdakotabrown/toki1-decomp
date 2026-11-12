/* @SymbolStart: 0x80137280 */
/* @RodataBase: 0x8015B744 */
/* Address: 0x80137288 */
/* Size: 288 bytes (72 insns) */
/* @SymbolSize: 296 */
#include "types.h"

extern void strcpy(s32, s32);
extern void rpg_bat_msg_set(s32, s32, s32);
extern void rpg_bat_koe_set(s32, s32, s32);
extern void rpg_bat_talk_wait(s32);
extern void rpg_bat_talk_clear(void);
extern void rpg_bat_seq_step_next(void);

extern s32 rpg_bat_talk_work_step;
extern u8  rpg_bat_msg_buf2[];
extern u8  rpg_bat_msg_buf2_1[];
extern u8  rpg_bat_msg_tbl2[];
extern u8  rpg_bat_furyou_talk_step_str0[];
extern u8  rpg_bat_furyou_talk_step_str1[];

void rpg_bat_furyou_talk_step(void) {
    switch (rpg_bat_talk_work_step) {
    case 0:
        strcpy((s32)rpg_bat_msg_buf2, (s32)rpg_bat_furyou_talk_step_str0);
        strcpy((s32)rpg_bat_msg_buf2_1, (s32)rpg_bat_furyou_talk_step_str1);
        rpg_bat_msg_set((s32)rpg_bat_msg_tbl2, 2, 200);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 1:
        rpg_bat_koe_set(0x1D000007, 0x0300000C, 0);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 2:
        rpg_bat_talk_wait(1);
        break;
    case 3:
        rpg_bat_koe_set(0x0E000045, 0x0B00004D, 1);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
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
