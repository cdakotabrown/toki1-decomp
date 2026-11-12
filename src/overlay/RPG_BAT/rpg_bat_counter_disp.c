/* rpg_bat_counter_disp - RPG_BAT overlay, Address: 0x8014B294, Size: 1188 bytes */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 k_disp_switch();
extern void rpg_bat_anim_set();

extern s32 rpg_bat_mode;
extern s32 rpg_bat_girl;

typedef struct {
    u8 f0;
    u8 f1;
    u8 f2;
    u8 f3;
    u8 rest[64];
} SpriteRec;

extern SpriteRec cw118_active[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit2;
extern RPG_BAT_UNIT rpg_bat_unit3;

void rpg_bat_counter_disp(void)
{
    s32 i;
    s32 place;

    place = 1;
    for (i = 0; i != 4; i++) {
        k_disp_switch(rpg_bat_unit0.disp, 1);
        if (rpg_bat_unit0.hp / place != 0) {
            rpg_bat_anim_set(19 - i, (rpg_bat_unit0.hp / place) % 10 + 9, 1, 1);
        } else {
            rpg_bat_anim_set(19 - i, 255, 1, 0);
        }

        if (rpg_bat_unit2.hp >= 0x10000001) {
            if (rpg_bat_girl != 14) {
                k_disp_switch(rpg_bat_unit2.disp, 0);
            }
            cw118_active[11].f3 = 0;
            cw118_active[1 - i].f3 = 0;
        } else {
            k_disp_switch(rpg_bat_unit2.disp, 1);
            cw118_active[11].f3 = 128;
            if (rpg_bat_unit2.hp / place != 0) {
                rpg_bat_anim_set(23 - i, (rpg_bat_unit2.hp / place) % 10 + 9, 1, 1);
            } else {
                rpg_bat_anim_set(23 - i, 255, 1, 0);
            }
        }

        if (rpg_bat_unit3.hp >= 0x10000001) {
            if ((rpg_bat_mode & 0xF0000) != 0) {
                k_disp_switch(rpg_bat_unit3.disp, 0);
            }
            if (rpg_bat_girl != 14) {
                cw118_active[10].f3 = 0;
                cw118_active[5 - i].f3 = 0;
            } else {
                cw118_active[11].f3 = 0;
                cw118_active[1 - i].f3 = 0;
            }
        } else {
            k_disp_switch(rpg_bat_unit3.disp, 1);
            if (rpg_bat_girl != 14) {
                cw118_active[10].f3 = 128;
                if (rpg_bat_unit3.hp / place != 0) {
                    rpg_bat_anim_set(27 - i, (rpg_bat_unit3.hp / place) % 10 + 9, 1, 1);
                } else {
                    rpg_bat_anim_set(27 - i, 255, 1, 0);
                }
            } else {
                cw118_active[11].f3 = 128;
                if (rpg_bat_unit3.hp / place != 0) {
                    rpg_bat_anim_set(23 - i, (rpg_bat_unit3.hp / place) % 10 + 9, 1, 1);
                } else {
                    rpg_bat_anim_set(27 - i, 255, 1, 0);
                }
            }
        }
        place = place * 10;
    }

    if (rpg_bat_unit0.hp == 0) {
        k_disp_switch(rpg_bat_unit0.disp, 1);
        rpg_bat_anim_set(19, 9, 1, 1);
    }
}
