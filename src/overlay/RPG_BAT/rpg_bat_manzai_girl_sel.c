/* @SymbolStart: 0x80150AA4 */
/* @RodataBase: 0x8015D5C0 */
/* @SymbolSize: 236 */
#include "types.h"

extern s32 rpg_bat_girl;

extern void rpg_bat_load_step(void);
extern void rpg_bat_step4(void);
extern void rpg_bat_step2(void);
extern void rpg_bat_load6(void);
extern void rpg_bat_phase6(void);
extern void kosiki_manzai_step(void);
extern void rpg_bat_step_sel(void);
extern void rpg_bat_cd_step(void);
extern void rpg_bat_asahina_talk_step(void);
extern void rpg_bat_battle_step(void);
extern void rpg_bat_phase6b(void);

void rpg_bat_manzai_girl_sel(void) {
    switch (rpg_bat_girl) {
    case 0:
        rpg_bat_load_step();
        break;
    case 1:
        rpg_bat_step4();
        break;
    case 2:
        rpg_bat_step2();
        break;
    case 3:
        rpg_bat_load6();
        break;
    case 4:
        rpg_bat_phase6();
        break;
    case 5:
        kosiki_manzai_step();
        break;
    case 6:
        rpg_bat_step_sel();
        break;
    case 7:
        rpg_bat_cd_step();
        break;
    case 8:
        rpg_bat_asahina_talk_step();
        break;
    case 9:
        rpg_bat_battle_step();
        break;
    case 10:
        rpg_bat_phase6b();
        break;
    }
}
