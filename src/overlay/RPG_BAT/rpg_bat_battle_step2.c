/* @SymbolStart: 0x801484E0 */
/* @RodataBase: 0x8015CB58 */
/* Address: 0x801484E8 */
/* Size: 364 bytes (91 insns) */
/* @SymbolSize: 372 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"


extern void strcpy(s32, s32);
extern void rpg_bat_msg_set(s32, s32, s32);
extern void rpg_bat_target_set(void);
extern void rpg_bat_pos1_set(s32);
extern void rpg_bat_reward_set(void);
extern void rpg_bat_step_next(void);
extern void rpg_bat_slide1(void);
extern void rpg_bat_wait(void);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_timer_step(s32);
extern void rpg_bat_rec_step(s32);
extern void rpg_bat_gauge_step(void);
extern void rpg_bat_bukatu7_arc_step(void);
extern void rpg_bat_bukatu_exit(void);
extern void rpg_bat_clear(void);

extern u8  rpg_bat_msg_buf[];
extern u8  rpg_bat_battle_step2_str0[];
extern u8  rpg_bat_msg_tbl[];
extern s32 rpg_bat_target_no;
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_battle_step2(void) {
    switch (rpg_bat_unit0.step) {
    case 0:
        strcpy((s32)rpg_bat_msg_buf, (s32)rpg_bat_battle_step2_str0);
        rpg_bat_msg_set((s32)rpg_bat_msg_tbl, 1, 90);
        rpg_bat_target_set();
        rpg_bat_pos1_set(rpg_bat_target_no);
        rpg_bat_reward_set();
        rpg_bat_step_next();
        rpg_bat_step_next();
        break;
    case 2:
        rpg_bat_slide1();
        break;
    case 3:
        rpg_bat_wait();
        break;
    case 4:
        rpg_bat_anim_set(52, 3, 1, 1);
        rpg_bat_step_next();
        break;
    case 5:
        rpg_bat_timer_step(32);
        break;
    case 6:
        rpg_bat_anim_set(52, 5, 5, 1);
        rpg_bat_step_next();
        break;
    case 7:
        rpg_bat_rec_step(52);
        break;
    case 8:
        rpg_bat_gauge_step();
        break;
    case 9:
        rpg_bat_bukatu7_arc_step();
        break;
    case 10:
        rpg_bat_timer_step(20);
        break;
    case 11:
        rpg_bat_bukatu_exit();
        break;
    case 12:
        rpg_bat_clear();
        break;
    }
}
