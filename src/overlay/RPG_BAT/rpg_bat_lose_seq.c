/* @SymbolStart: 0x80159620 */
/* @RodataBase: 0x8015E32C */
/* @SymbolSize: 200 */
/* Address: 0x80159628, Size: 192 bytes (48 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"


extern s32 sub_step_up(void);
extern s32 device_port_init(s32 a0, s32 a1);
extern void rpg_bat_seq_step_next(void);
extern void rpg_bat_seq_wait(s32 a0);
extern void rpg_bat_seq_step_gate(s32 a0);
extern void banchou_lose_scorn_talk(void);

extern s32 rpg_bat_result;
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

void rpg_bat_lose_seq(void) {
    switch (rpg_bat_unit1.step) {
    case 0:
        rpg_bat_unit0.flag = 4096;
        rpg_bat_seq_step_gate(0);
        break;
    case 1:
        rpg_bat_unit0.flag = 4096;
        rpg_bat_seq_step_gate(0);
        break;
    case 2:
        rpg_bat_seq_wait(20);
        break;
    case 3:
        device_port_init(1, 515);
        rpg_bat_seq_step_next();
        break;
    case 4:
        banchou_lose_scorn_talk();
        break;
    case 5:
        rpg_bat_result = 1;
        sub_step_up();
        break;
    }
}
