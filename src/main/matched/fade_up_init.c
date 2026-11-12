/* Address: 0x8007E53C, Size: 112 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: scheduler_normalizer */
#include "types.h"
#include "tokimeki/sprite.h"
extern u8  bustup2_cw_bright;
extern u8  bg_change_tbl2;
extern s32 bustup_on;
extern s32 fade_flag;
extern s32 step_counter;
extern SPRITE_REC Sprites[];
s32 fade_up_init(void) {
    bustup2_cw_bright  = 0;
    (*(s8 *)&Sprites[0].g)  = 0;
    (*(s8 *)&Sprites[0].r)  = 0;
    (*(s8 *)&Sprites[1].g)  = 0;
    (*(s8 *)&Sprites[1].r)  = 0;
    bg_change_tbl2  = 0;
    fade_flag  = 0;
    bustup_on  = 1;
    step_counter  = step_counter + 1;
    Sprites[0].b  = 0;
    Sprites[1].b  = 0;
    return 0;
}
