/* @SymbolStart: 0x80139460 */
/* @SymbolSize: 176 */
/* rpg_bat_furyou_end_step - RPG_BAT overlay, Address: 0x80139468, Size: 176 bytes (44 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"


extern s32 rpg_bat_result;
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

extern s32 sub_step_up();
extern s32 device_port_init();
extern void rpg_bat_furyou_talk_step2();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_seq_step_next();
extern s32 rpg_bat_seq_wait();

void rpg_bat_furyou_end_step(void)
{
    switch (rpg_bat_unit1.step) {
    case 0:
        rpg_bat_unit0.flag = 4096;
        rpg_bat_anim_set(48, 7, 1, 1);
        device_port_init(1, 515);
        rpg_bat_seq_step_next();
        break;
    case 1:
        rpg_bat_seq_wait(120);
        break;
    case 2:
        rpg_bat_furyou_talk_step2();
        break;
    case 3:
        rpg_bat_result = 1;
        sub_step_up();
        break;
    }
}
