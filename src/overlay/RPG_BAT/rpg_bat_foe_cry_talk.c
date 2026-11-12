/* @SymbolStart: 0x801361AC */
/* @RodataBase: 0x8015AF60 */
/* Address: 0x801361B4 */
/* Size: 448 bytes (112 insns) */
/* @SymbolSize: 456 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern void strcpy(s32, s32);
extern void rpg_bat_msg_set(s32, s32, s32);
extern void rpg_bat_koe_set(s32, s32, s32);
extern void rpg_bat_talk_wait(s32);
extern void rpg_bat_talk_clear(void);
extern void rpg_bat_act_next(void);

extern s32 rpg_bat_talk_work_step;
extern u8  rpg_bat_msg_buf3[];
extern u8  rpg_bat_msg_buf3_1[];
extern u8  rpg_bat_msg_tbl3[];
extern u8  rpg_bat_talk2_str0[];
extern u8  rpg_bat_talk2_str1[];
extern u8  rpg_bat_talk2_str2[];
extern u8  rpg_bat_talk2_str3[];
extern RPG_BAT_UNIT rpg_bat_unit1;

void rpg_bat_foe_cry_talk(void) {
    switch (rpg_bat_talk_work_step) {
    case 0:
        if (rpg_bat_unit1.flag & 0x100000) {
            strcpy((s32)rpg_bat_msg_buf3, (s32)rpg_bat_talk2_str0);
            strcpy((s32)rpg_bat_msg_buf3_1, (s32)rpg_bat_talk2_str1);
        } else {
            strcpy((s32)rpg_bat_msg_buf3, (s32)rpg_bat_talk2_str2);
            strcpy((s32)rpg_bat_msg_buf3_1, (s32)rpg_bat_talk2_str3);
        }
        rpg_bat_msg_set((s32)rpg_bat_msg_tbl3, 2, 200);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 1:
        if (rpg_bat_unit1.flag & 0x100000) {
            rpg_bat_koe_set(0x1D000014, 0x05000017, 0);
        } else {
            rpg_bat_koe_set(0x1E00008C, 0x1B000093, 0);
        }
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 2:
        rpg_bat_talk_wait(1);
        break;
    case 3:
        if (rpg_bat_unit1.flag & 0x100000) {
            rpg_bat_koe_set(0x0E00004E, 0x0C000057, 1);
        } else {
            rpg_bat_koe_set(0x04000012, 0x04000020, 1);
        }
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 4:
        rpg_bat_talk_wait(0);
        break;
    case 5:
        rpg_bat_talk_clear();
        rpg_bat_act_next();
        break;
    }
}
