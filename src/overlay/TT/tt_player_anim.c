/* @CompileEnv: IDO52_FORCE_WEB_COLOR=10:a0,13:a1,28:v1 */
/* tt_player_anim - TT overlay, Address: 0x8013F978, Size: 88 bytes (22 insns) */
#include "types.h"

extern u8 *tt_boss_wk;

void tt_player_anim(void)
{
    u8 *base;
    s32 i;

    base = tt_boss_wk;
    if ((*(u16 *)(base + 870) & 7) == 7) {
        for (i = 320; i != 380; i += 20) {
            if (++base[i + 62] >= 22U) {
                base[i + 62] = 16;
            }
        }
    }
}
