/* @SymbolSize: 64 */
/* rpg_bat_step_sel - RPG_BAT overlay, Address: 0x80141C30, Size: 64 bytes (16 insns) */
#include "types.h"

extern s32 bat_zyotai_check();
extern void kiyokawa_manzai_step();
extern void kiyokawa_manzai_step2();

void rpg_bat_step_sel(void)
{
    if (bat_zyotai_check() != 0) {
        kiyokawa_manzai_step();
    } else {
        kiyokawa_manzai_step2();
    }
}
