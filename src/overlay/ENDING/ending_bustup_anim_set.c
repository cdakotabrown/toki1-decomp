/* @SymbolSize: 52 */
/* ending_bustup_anim_set - ENDING overlay, Address: 0x801396CC, Size: 52 bytes (13 instructions) */
#include "types.h"
#include "tokimeki/class_w.h"

extern s16 bustup_cw_anim;
extern s16 bustup_cw_frame;
extern u8 bustup_cw_mode;
extern CLASS_W class_w[];

void ending_bustup_anim_set(s16 arg0, u8 arg1)
{
    bustup_cw_anim = arg0;
    bustup_cw_frame = 0;
    class_w[30].tick = 0;
    if (arg1 != 255) {
        bustup_cw_mode = arg1;
    }
}
