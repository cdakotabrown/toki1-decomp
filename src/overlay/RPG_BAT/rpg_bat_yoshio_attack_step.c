/* @SymbolStart: 0x801340F0 */
/* @SymbolSize: 108 */
/* rpg_bat_yoshio_attack_step - RPG_BAT overlay, Address: 0x801340F8, Size: 100 bytes (25 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit2;


extern void rpg_bat_seq1();
extern void rpg_bat_seq2();
extern void rpg_bat_yoshio_hiyaku_step();

void rpg_bat_yoshio_attack_step(void)
{
    switch (rpg_bat_unit2.flag) {
    case 1:
        rpg_bat_seq1();
        break;
    case 2:
        rpg_bat_seq2();
        break;
    case 3:
        rpg_bat_yoshio_hiyaku_step();
        break;
    }
}
