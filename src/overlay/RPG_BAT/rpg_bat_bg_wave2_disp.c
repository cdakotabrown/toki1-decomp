/* @PostProc: patch_insn_reg_field 0xe4:rs:t9:t2 0xe4:rt:t2:t9 0x1b8:rd:t9:t2 0x1bc:rs:t8:t2 0x1bc:rt:t9:t8 0x1c8:rt:t2:t9 0x1cc:rs:t2:t9 0x1cc:rt:t2:t9 0x1d4:rt:t2:t9 */
/* rpg_bat_bg_wave2_disp - RPG_BAT overlay, Address: 0x80153D20, Size: 540 bytes (135 insns) */
#include "types.h"

extern s32 rpg_bat_bg_wave2_phase;
extern s32 rpg_bat_bg_wave2_amp;
extern s32 rpg_bat_bg_wave2_period;
extern s32 rpg_bat_bg_wave2_len;

extern s32 rsin(s32 arg);
extern s32 rpg_bat_bg_quad_disp(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

void rpg_bat_bg_wave2_disp(void)
{
    s32 i;
    s32 r0, r1;
    s32 v0;

    for (i = 0; i != 170; i += 5) {
        v0 = rsin(((rpg_bat_bg_wave2_phase << 12) / rpg_bat_bg_wave2_period) +
                           (((i % rpg_bat_bg_wave2_len) << 12) / rpg_bat_bg_wave2_len));
        r0 = (v0 * rpg_bat_bg_wave2_amp) >> 13;
        v0 = rsin(((rpg_bat_bg_wave2_phase << 12) / rpg_bat_bg_wave2_period) +
                           ((((i + 4) % rpg_bat_bg_wave2_len) << 12) / rpg_bat_bg_wave2_len));
        r1 = (v0 * rpg_bat_bg_wave2_amp) >> 13;
        rpg_bat_bg_quad_disp(r0, r1, i, i + 4, 2, 5);
    }
}
