/* @SymbolStart: 0x801374D0 */
/* @RodataBase: 0x8015B774 */
/* Address: 0x801374D8 */
/* Size: 288 bytes (72 insns) */
/* @SymbolSize: 296 */
#include "types.h"

extern void strcpy(s32, s32);
extern void rpg_bat_msg_set(s32, s32, s32);
extern void rpg_bat_koe_set(s32, s32, s32);
extern void rpg_bat_talk_wait(s32);
extern void rpg_bat_talk_clear(void);
extern void rpg_bat_seq_next(void);

extern s32 rpg_bat_talk_work_step;
extern u8  rpg_bat_msg_buf4[];
extern u8  rpg_bat_furyou_talk_step3_str0[];
extern u8  rpg_bat_msg4_line1[];
extern u8  rpg_bat_furyou_talk_step3_str1[];
extern u8  rpg_bat_msg_tbl4[];

void rpg_bat_furyou_talk_step3(void) {
    switch (rpg_bat_talk_work_step) {
    case 0:
        strcpy((s32)rpg_bat_msg_buf4, (s32)rpg_bat_furyou_talk_step3_str0);
        strcpy((s32)rpg_bat_msg4_line1, (s32)rpg_bat_furyou_talk_step3_str1);
        rpg_bat_msg_set((s32)rpg_bat_msg_tbl4, 2, 260);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 1:
        rpg_bat_koe_set(0x0E000014, 0x0700001C, 0);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 2:
        rpg_bat_talk_wait(1);
        break;
    case 3:
        rpg_bat_koe_set(0x0E000037, 0x0A000044, 1);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 4:
        rpg_bat_talk_wait(0);
        break;
    case 5:
        rpg_bat_talk_clear();
        rpg_bat_seq_next();
        break;
    }
}
