/* @SymbolStart: 0x8013794C */
/* @RodataBase: 0x8015B7C4 */
/* @SymbolSize: 380 */
/* Address: 0x80137954, Size: 372 bytes (93 insns) */
#include "types.h"

extern s32 strcpy(s32 a0, s32 a1);
extern s32 rpg_bat_koe_set(s32 a0, s32 a1, s32 a2);
extern s32 rpg_bat_talk_clear(void);
extern s32 rpg_bat_talk_wait(s32 a0);
extern s32 rpg_bat_msg_set(s32 a0, s32 a1, s32 a2);
extern s32 rpg_bat_seq_next(void);

extern s32 rpg_bat_talk_work_step;
extern u8 banchou_taunt_talk_str0[];
extern u8 banchou_taunt_talk_str1[];
extern u8 banchou_taunt_talk_str2[];
extern u8 rpg_bat_msg_buf4[];
extern u8 rpg_bat_msg4_line1[];
extern u8 rpg_bat_msg4_line2[];
extern u8 rpg_bat_msg_tbl4[];

void banchou_taunt_talk(void) {
    switch (rpg_bat_talk_work_step) {
    case 0:
        strcpy((s32)rpg_bat_msg_buf4, (s32)banchou_taunt_talk_str0);
        strcpy((s32)rpg_bat_msg4_line1, (s32)banchou_taunt_talk_str1);
        strcpy((s32)rpg_bat_msg4_line2, (s32)banchou_taunt_talk_str2);
        rpg_bat_msg_set((s32)rpg_bat_msg_tbl4, 3, 280);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 1:
        rpg_bat_koe_set(0x1A00005B, 0x0F000065, 0);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 2:
        rpg_bat_talk_wait(1);
        break;
    case 3:
        rpg_bat_koe_set(0x0300004C, 0x0900005B, 1);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 4:
        rpg_bat_talk_wait(2);
        break;
    case 5:
        rpg_bat_koe_set(0x1A000066, 0x10000073, 2);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
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
