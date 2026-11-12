/* @SymbolSize: 676 */
/* rpg_bat_wave_disp - RPG_BAT overlay, Address: 0x8013C32C, Size: 676 bytes (169 insns) */
#include "types.h"

extern s32 rpg_bat_mode;
extern s32 rpg_bat_pause_mode;
extern s32 rpg_bat_bg_wave_phase;
extern s32 rpg_bat_bg_wave_amp;
extern s32 rpg_bat_bg_wave_period;
extern s32 rpg_bat_bg_wave_len;
extern s32 rpg_bat_bg_wave_step;
extern s32 rpg_bat_bg_wave_on;

extern s32 rsin();
extern void rpg_bat_bg_quad_disp();

void rpg_bat_wave_disp(void)
{
    s32 i;
    s32 j;
    s32 y0;

    if (rpg_bat_bg_wave_on != 1) {
        return;
    }
    if ((rpg_bat_mode & 0x100000) == 0) {
        return;
    }
    if (rpg_bat_pause_mode != 3) {
        rpg_bat_bg_wave_phase = rpg_bat_bg_wave_phase + rpg_bat_bg_wave_step;
        if (rpg_bat_bg_wave_phase >= (rpg_bat_bg_wave_period << 1)) {
            rpg_bat_bg_wave_phase = 0;
        }
    }
    for (i = 0; i != 168; i += 8) {
        y0 = (rsin(((i % rpg_bat_bg_wave_len) << 12) / rpg_bat_bg_wave_len +
                            ((rpg_bat_bg_wave_phase << 12) / 2) / rpg_bat_bg_wave_period) *
              rpg_bat_bg_wave_amp) >> 13;
        j = i + 7;
        rpg_bat_bg_quad_disp(y0,
                      (rsin(((j % rpg_bat_bg_wave_len) << 12) / rpg_bat_bg_wave_len +
                                     ((rpg_bat_bg_wave_phase << 12) / 2) / rpg_bat_bg_wave_period) *
                       rpg_bat_bg_wave_amp) >> 13,
                      i, j, 0, 0);
    }
}
