/* @SymbolStart: 0x80158A0C */
/* @RodataBase: 0x8015E280 */
/* @SymbolSize: 356 */
/* banchou_talk_seq - RPG_BAT overlay, Address: 0x80158A14, Size: 348 bytes (87 insns) */
#include "types.h"

extern s32 rpg_bat_unit1_step;

extern s32 rpg_bat_seq_work;
extern u32 rpg_bat_unit1;
extern u32 rpg_bat_unit1_hp_max;

extern s32 get_rand_mod();
extern s32 rpg_bat_seq_step_next();
extern s32 rpgbat_talk0b();
extern s32 rpgbat_talk0a();
extern s32 banchou_hp_band1_talk();
extern s32 banchou_hp_band2_talk();
extern s32 banchou_hp_band3_talk();
extern s32 banchou_hp_band4_talk();
extern s32 rpg_bat_seq_wait();
extern s32 banchou_provoke_talk();
extern s32 rpg_bat_effect_clear();

void banchou_talk_seq(void)
{
    u32 idx;

    switch (rpg_bat_unit1_step) {
    case 0:
        rpg_bat_seq_work = get_rand_mod(2);
        rpg_bat_seq_step_next();
        break;
    case 1:
        idx = (rpg_bat_unit1 * 6) / rpg_bat_unit1_hp_max;
        switch (idx) {
        case 0:
            if (rpg_bat_seq_work == 0) {
                rpgbat_talk0a();
            } else {
                rpgbat_talk0b();
            }
            break;
        case 1:
            banchou_hp_band1_talk();
            break;
        case 2:
            banchou_hp_band2_talk();
            break;
        case 3:
            banchou_hp_band3_talk();
            break;
        case 4:
            banchou_hp_band4_talk();
            break;
        case 5:
            banchou_provoke_talk();
            break;
        case 6:
            banchou_provoke_talk();
            break;
        }
        break;
    case 2:
        rpg_bat_seq_wait(20);
        break;
    case 3:
        rpg_bat_effect_clear();
        break;
    }
}
