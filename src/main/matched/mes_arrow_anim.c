/* Address: 0x8006C9DC */
/* Size: 84 bytes (21 instructions) */
/* Region: Game (IDO 5.2 -O2, -Wc,-pic1) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s16 yazirushi_cw_y;
extern s32 frame_counter;

mes_arrow_anim()
{
    if (yazirushi_cw_y < 79) {
        if ((frame_counter & 3) == 0) {
            yazirushi_cw_y++;
        }
    } else {
        yazirushi_cw_y = 72;
    }
}
