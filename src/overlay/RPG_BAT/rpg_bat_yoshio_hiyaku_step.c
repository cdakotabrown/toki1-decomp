/* @SymbolStart: 0x80134984 */
/* @RodataBase: 0x8015AD14 */
/* @SymbolSize: 232 */
/* rpg_bat_yoshio_hiyaku_step - RPG_BAT overlay, Address: 0x8013498C, Size: 224 bytes (56 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"


extern s32 strcpy();
extern s32 rpg_bat_slot_adjust();
extern s32 rpg_bat_se_set();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_act_next();
extern s32 rpg_bat_reset();
extern s32 rpg_bat_act_wait();
extern s32 rpg_bat_act_gate();
extern char rpg_bat_yoshio_hiyaku_step_str0[];
extern char rpg_bat_msg_buf3[];
extern char rpg_bat_msg_tbl3[];
extern RPG_BAT_UNIT rpg_bat_unit2;

void rpg_bat_yoshio_hiyaku_step(void)
{
    s32 t7;

    switch (rpg_bat_unit2.step) {
    case 0:
        strcpy(rpg_bat_msg_buf3, rpg_bat_yoshio_hiyaku_step_str0);
        rpg_bat_msg_set(rpg_bat_msg_tbl3, 1, 60);
        rpg_bat_act_next();
        break;
    case 1:
        rpg_bat_act_gate(1);
        break;
    case 2:
        rpg_bat_slot_adjust(2, 35, 9999, 0);
        t7 = rpg_bat_unit2.hp_max;
        rpg_bat_unit2.hp = t7;
        rpg_bat_se_set(1282, 1, 0);
        rpg_bat_act_next();
        break;
    case 3:
        rpg_bat_act_wait(30);
        break;
    case 4:
        rpg_bat_reset();
        break;
    default:
        break;
    }
}
