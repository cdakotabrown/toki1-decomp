/* @SymbolStart: 0x8013D0D4 */
/* @RodataBase: 0x8015BC60 */
/* @SymbolSize: 252 */
/* rpg_bat_bukatu_attack_sel - RPG_BAT overlay, Address: 0x8013D0DC, Size: 252 bytes (63 insns) */
#include "types.h"

extern s32 rpg_bat_bukatu_no;

extern void rpg_bat_niagara_dunk_step(void);
extern void rpg_bat_step(void);
extern void rpg_bat_step3(void);
extern void rpg_bat_cyber_fang_step(void);
extern void rpg_bat_state(void);
extern void rpg_bat_battle_step2(void);
extern void rpg_bat_scene_step(void);
extern void rpg_bat_bukatu9_attack(void);
extern void rpg_bat_bukatu1_attack(void);
extern void rpg_bat_bukatu4_attack(void);
extern void rpg_bat_bukatu8_attack(void);
extern void rpg_bat_bukatu6_attack(void);

void rpg_bat_bukatu_attack_sel(void)
{
    switch (rpg_bat_bukatu_no) {
    case 0:  rpg_bat_step3(); break;
    case 1:  rpg_bat_bukatu1_attack(); break;
    case 2:  rpg_bat_state(); break;
    case 3:  rpg_bat_cyber_fang_step(); break;
    case 4:  rpg_bat_bukatu4_attack(); break;
    case 5:  rpg_bat_step(); break;
    case 6:  rpg_bat_bukatu6_attack(); break;
    case 7:  rpg_bat_battle_step2(); break;
    case 8:  rpg_bat_bukatu8_attack(); break;
    case 9:  rpg_bat_bukatu9_attack(); break;
    case 10: rpg_bat_niagara_dunk_step(); break;
    case 11: rpg_bat_scene_step(); break;
    default: break;
    }
}
