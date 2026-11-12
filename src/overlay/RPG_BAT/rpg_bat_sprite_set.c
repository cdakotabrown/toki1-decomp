/* @SymbolSize: 632 */
/* rpg_bat_sprite_set - RPG_BAT overlay, Address: 0x8015785C, Size: 632 bytes (158 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 get_rand_mod();

extern s32 rpg_bat_bg_wave_phase;
extern s32 rpg_bat_bg_wave_amp;
extern s32 rpg_bat_bg_wave_period;
extern s32 rpg_bat_bg_wave_len;
extern s32 rpg_bat_bg_wave_step;
extern s32 rpg_bat_bg_wave_on;
extern CLASS_W class_w[];

void rpg_bat_sprite_set(void)
{
    rpg_bat_sprite_pos_set(47, rpg_bat_bg_wave_phase, rpg_bat_bg_wave_amp);
    rpg_bat_sprite_pos_set(46, rpg_bat_bg_wave_phase, rpg_bat_bg_wave_amp);
    rpg_bat_sprite_pos_set(42, rpg_bat_bg_wave_phase - 103, rpg_bat_bg_wave_amp - 6);
    rpg_bat_sprite_pos_set(43, rpg_bat_bg_wave_phase - 67, rpg_bat_bg_wave_amp + 6);
    rpg_bat_sprite_pos_set(44, rpg_bat_bg_wave_phase + 67, rpg_bat_bg_wave_amp + 6);
    rpg_bat_sprite_pos_set(45, rpg_bat_bg_wave_phase + 103, rpg_bat_bg_wave_amp - 6);

    if ((class_w[138].anim_flag & 1) == 0) {
        rpg_bat_anim_set2(42, 255, 1, 0);
        rpg_bat_bg_wave_period = 0;
    }
    if ((class_w[139].anim_flag & 1) == 0) {
        rpg_bat_anim_set2(43, 255, 1, 0);
        rpg_bat_bg_wave_len = 0;
    }
    if ((class_w[140].anim_flag & 1) == 0) {
        rpg_bat_anim_set2(44, 255, 1, 0);
        rpg_bat_bg_wave_step = 0;
    }
    if ((class_w[141].anim_flag & 1) == 0) {
        rpg_bat_anim_set2(45, 255, 1, 0);
        rpg_bat_bg_wave_on = 0;
    }

    if (rpg_bat_bg_wave_period == 0 && get_rand_mod(100) == 0) {
        rpg_bat_anim_set2(42, 7, 5, 1);
        rpg_bat_bg_wave_period = 1;
    }
    if (rpg_bat_bg_wave_len == 0 && get_rand_mod(100) == 0) {
        rpg_bat_anim_set2(43, 7, 5, 1);
        rpg_bat_bg_wave_len = 1;
    }
    if (rpg_bat_bg_wave_step == 0 && get_rand_mod(100) == 0) {
        rpg_bat_anim_set2(44, 7, 5, 1);
        rpg_bat_bg_wave_step = 1;
    }
    if (rpg_bat_bg_wave_on == 0 && get_rand_mod(100) == 0) {
        rpg_bat_anim_set2(45, 7, 5, 1);
        rpg_bat_bg_wave_on = 1;
    }
}
