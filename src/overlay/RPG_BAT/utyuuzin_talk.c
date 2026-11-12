/* @SymbolStart: 0x80156CC0 */
/* @SymbolSize: 184 */
/* utyuuzin_talk - RPG_BAT overlay, Address: 0x80156CC8, Size: 176 bytes (44 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"


extern s32 strcpy();
extern s32 rpg_bat_se_set();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_seq_step_next();
extern s32 rpg_bat_effect_clear();
extern s32 rpg_bat_seq_wait();
extern s32 rpg_bat_seq_step_gate();

extern u8 utyuuzin_talk_str0[];
extern u8 rpg_bat_msg_buf2[];
extern u8 rpg_bat_msg_tbl2[];
extern RPG_BAT_UNIT rpg_bat_unit1;

void utyuuzin_talk(void)
{
    switch (rpg_bat_unit1.step) {
    case 0:
        strcpy(rpg_bat_msg_buf2, utyuuzin_talk_str0);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 90);
        rpg_bat_se_set(1298, 1, 0);
        rpg_bat_seq_step_next();
        break;
    case 1:
        rpg_bat_seq_step_gate(0);
        break;
    case 2:
        rpg_bat_seq_wait(20);
        break;
    case 3:
        rpg_bat_effect_clear();
        break;
    }
}
