/* @SymbolStart: 0x80137F50 */
/* @RodataBase: 0x8015B824 */
/* Address: 0x80137F58 */
/* Size: 456 bytes (114 insns) */
/* @SymbolSize: 464 */
#include "types.h"

extern void strcpy(s32, s32);
extern void rpg_bat_msg_set(s32, s32, s32);
extern void rpg_bat_koe_set(s32, s32, s32);
extern void rpg_bat_talk_wait(s32);
extern void rpg_bat_talk_clear(void);
extern void rpg_bat_seq_next(void);

extern s32 rpg_bat_talk_work_step;
extern u8  rpg_bat_msg_buf4[];
extern u8  rpg_bat_msg4_line1[];
extern u8  rpg_bat_msg4_line2[];
extern u8  rpg_bat_msg4_line3[];
extern u8  rpg_bat_msg_tbl4[];
extern u8  banchou_power_talk_str0[];
extern u8  banchou_power_talk_str1[];
extern u8  banchou_power_talk_str2[];
extern u8  banchou_power_talk_str3[];

void banchou_power_talk(void) {
    switch (rpg_bat_talk_work_step) {
    case 0:
        strcpy((s32)rpg_bat_msg_buf4, (s32)banchou_power_talk_str0);
        strcpy((s32)rpg_bat_msg4_line1, (s32)banchou_power_talk_str1);
        strcpy((s32)rpg_bat_msg4_line2, (s32)banchou_power_talk_str2);
        strcpy((s32)rpg_bat_msg4_line3, (s32)banchou_power_talk_str3);
        rpg_bat_msg_set((s32)rpg_bat_msg_tbl4, 4, 280);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 1:
        rpg_bat_koe_set(0x1A00007B, 0x1200007F, 0);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 2:
        rpg_bat_talk_wait(1);
        break;
    case 3:
        rpg_bat_koe_set(0x1A000080, 0x1300008D, 1);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 4:
        rpg_bat_talk_wait(2);
        break;
    case 5:
        rpg_bat_koe_set(0x0300005C, 0x0A00006A, 2);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 6:
        rpg_bat_talk_wait(3);
        break;
    case 7:
        rpg_bat_koe_set(0x1E00009F, 0x1D0000A4, 3);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 8:
        rpg_bat_talk_wait(0);
        break;
    case 9:
        rpg_bat_talk_clear();
        rpg_bat_seq_next();
        break;
    }
}
