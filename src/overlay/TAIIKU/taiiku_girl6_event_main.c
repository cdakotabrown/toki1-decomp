/* Address: 0x80141E58 */
/* Size: 84 bytes (21 insns) */
#include "types.h"

extern u8 now_sub_step;
extern u8 now_sub_sub_step;
extern s32 taiiku_end;

extern s32 taiiku_girl6_event();
extern s32 game_reset();

void taiiku_girl6_event_main(void)
{
    s32 result;

    result = taiiku_girl6_event();
    if (result == 0) {
        return;
    }
    if (result == 1) {
        now_sub_step = 0;
        now_sub_sub_step = 0;
        taiiku_end = 1;
    } else {
        game_reset();
    }
}
