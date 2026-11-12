/* @SymbolStart: 0x80136070 */
/* @RodataBase: 0x8015AF48 */
/* Address: 0x80136078 */
/* Size: 308 bytes (77 insns) */
/* @SymbolSize: 316 */
#include "types.h"

extern void strcpy(s32, s32);
extern void rpg_bat_msg_set(s32, s32, s32);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_koe_set(s32, s32, s32);
extern void rpg_bat_talk_wait(s32);
extern void rpg_bat_talk_clear(void);
extern void rpg_bat_act_next(void);

extern s32 rpg_bat_talk_work_step;
extern u8  rpg_bat_msg_buf3[];
extern u8  rpg_bat_talk_str0[];
extern u8  rpg_bat_msg_buf3_1[];
extern u8  rpg_bat_talk_str1[];
extern u8  rpg_bat_msg_tbl3[];

void rpg_bat_talk(void) {
    switch (rpg_bat_talk_work_step) {
    case 0:
        strcpy((s32)rpg_bat_msg_buf3, (s32)rpg_bat_talk_str0);
        strcpy((s32)rpg_bat_msg_buf3_1, (s32)rpg_bat_talk_str1);
        rpg_bat_msg_set((s32)rpg_bat_msg_tbl3, 2, 180);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 1:
        rpg_bat_koe_set(0x0700002C, 0x07000034, 0);
        rpg_bat_talk_work_step = rpg_bat_talk_work_step + 1;
        break;
    case 2:
        rpg_bat_talk_wait(1);
        break;
    case 3:
        rpg_bat_anim_set(30, 7, 5, 1);
        rpg_bat_koe_set(0x04000029, 0x0600002E, 1);
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
