/* @SymbolStart: 0x801376D4 */
/* @RodataBase: 0x8015B78C */
/* @SymbolSize: 632 */
/* banchou_intro_talk - RPG_BAT overlay, Address: 0x801376DC, Size: 624 bytes (156 insns) */
#include "types.h"

extern s32 strcpy();
extern s32 rpg_bat_koe_set();
extern s32 rpg_bat_talk_clear();
extern s32 rpg_bat_talk_wait();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_seq_next();

extern char rpg_bat_msg_buf4[];
extern char rpg_bat_msg4_line1[];
extern char rpg_bat_msg4_line2[];
extern char rpg_bat_msg4_line3[];
extern char rpg_bat_msg4_line4[];
extern char rpg_bat_msg4_line5[];
extern char rpg_bat_msg_tbl4[];
extern char banchou_intro_talk_str0[];
extern char banchou_intro_talk_str1[];
extern char banchou_intro_talk_str2[];
extern char banchou_intro_talk_str3[];
extern char banchou_intro_talk_str4[];
extern char banchou_intro_talk_str5[];
extern s32 rpg_bat_talk_work_step;

void banchou_intro_talk(void)
{
    s32 next;

    switch ((u32)rpg_bat_talk_work_step) {
    case 0:
        strcpy((s32)rpg_bat_msg_buf4, (s32)banchou_intro_talk_str0);
        strcpy((s32)rpg_bat_msg4_line1, (s32)banchou_intro_talk_str1);
        strcpy((s32)rpg_bat_msg4_line2, (s32)banchou_intro_talk_str2);
        strcpy((s32)rpg_bat_msg4_line3, (s32)banchou_intro_talk_str3);
        strcpy((s32)rpg_bat_msg4_line4, (s32)banchou_intro_talk_str4);
        strcpy((s32)rpg_bat_msg4_line5, (s32)banchou_intro_talk_str5);
        rpg_bat_msg_set((s32)rpg_bat_msg_tbl4, 6, 270);
        next = rpg_bat_talk_work_step + 1;
        rpg_bat_talk_work_step = next;
        break;
    case 1:
        rpg_bat_koe_set(0x04000002, 0x03000011, 0);
        next = rpg_bat_talk_work_step + 1;
        rpg_bat_talk_work_step = next;
        break;
    case 2:
        rpg_bat_talk_wait(1);
        break;
    case 3:
        rpg_bat_koe_set(0x1A000096, 0x150000A3, 1);
        next = rpg_bat_talk_work_step + 1;
        rpg_bat_talk_work_step = next;
        break;
    case 4:
        rpg_bat_talk_wait(2);
        break;
    case 5:
        rpg_bat_koe_set(0x1A0000A4, 0x160000B0, 2);
        next = rpg_bat_talk_work_step + 1;
        rpg_bat_talk_work_step = next;
        break;
    case 6:
        rpg_bat_talk_wait(3);
        break;
    case 7:
        rpg_bat_koe_set(0x1B000082, 0x0800008A, 3);
        next = rpg_bat_talk_work_step + 1;
        rpg_bat_talk_work_step = next;
        break;
    case 8:
        rpg_bat_talk_wait(4);
        break;
    case 9:
        rpg_bat_koe_set(0x1B000095, 0x0A0000A0, 4);
        next = rpg_bat_talk_work_step + 1;
        rpg_bat_talk_work_step = next;
        break;
    case 10:
        rpg_bat_talk_wait(5);
        break;
    case 11:
        rpg_bat_koe_set(0x190000AA, 0x140000B7, 5);
        next = rpg_bat_talk_work_step + 1;
        rpg_bat_talk_work_step = next;
        break;
    case 12:
        rpg_bat_talk_wait(0);
        break;
    case 13:
        rpg_bat_talk_clear();
        rpg_bat_seq_next();
        break;
    }
}
