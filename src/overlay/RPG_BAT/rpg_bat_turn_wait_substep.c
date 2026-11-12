/* @SymbolStart: 0x80158B70 */
/* @RodataBase: 0x8015E29C */
/* @SymbolSize: 220 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"


extern s32 get_rpg_bat_turn_wait(s32 a0);
extern void rpg_bat_seq_step_next(void);
extern void rpg_bat_substep_next(void);
extern void rpg_bat_substep_wait(s32 a0);

extern s32 rpg_bat_seq_work[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

void rpg_bat_turn_wait_substep(void) {
    s32 v0;
    switch (rpg_bat_unit1.sub) {
    case 0:
        v0 = get_rpg_bat_turn_wait(0);
        rpg_bat_seq_work[1] = v0 - rpg_bat_unit0.aux08;
        if (rpg_bat_seq_work[1] <= 0) {
            rpg_bat_seq_work[1] = 1;
        }
        rpg_bat_substep_next();
        break;
    case 1:
        rpg_bat_substep_wait(rpg_bat_seq_work[1]);
        break;
    case 2:
        rpg_bat_unit0.flag |= 0x10000;
        rpg_bat_unit0.aux08 = 0;
        rpg_bat_substep_next();
        break;
    case 3:
        rpg_bat_substep_wait(90);
        break;
    case 4:
        rpg_bat_seq_step_next();
        break;
    }
}
