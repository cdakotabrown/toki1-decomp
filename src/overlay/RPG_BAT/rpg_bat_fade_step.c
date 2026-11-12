/* @SymbolStart: 0x8015461C */
/* @SymbolSize: 324 */
/* @CompileFlags: -cckr */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

typedef struct {
    u8 f0; u8 f1; u8 f2; u8 f3; u8 f4; u8 f5; u8 f6; u8 f7;
    s32 f8; s32 f12; s32 f16; u8 pad20[18]; s16 f38; s16 pad40; s16 f42;
    u8 pad44[8]; s32 f52; s32 f56; u8 pad60[7]; u8 f67;
} Rec;
extern Rec class_w[];

extern s32 rpg_bat_dmg;
extern s32 rpg_bat_enemy_hp;
extern s32 rpg_bat_pair_sel;
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

void rpg_bat_fade_step()
{
    s32 i;

    switch (rpg_bat_unit0.sub) {
    case 0:
        rpg_bat_sprite_bri_set(255);
        rpg_bat_state_next();
        break;
    case 1:
        rpg_bat_timer_sub(5);
        break;
    case 2:
        rpg_bat_sprite_bri_set(128);
        for (i = 0; i < 6; i++) {
            class_w[i + 136].f3 ^= 0x80;
        }
        if ((rpg_bat_unit1.flag & 0x100000) && rpg_bat_dmg == 1000) {
            rpg_bat_enemy_hp = 0;
        } else {
            class_w[rpg_bat_pair_sel + 96].f3 ^= 0x80;
            rpg_bat_row_disp();
        }
        rpg_bat_step_next();
        break;
    }
}
