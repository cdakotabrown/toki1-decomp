/* @SymbolSize: 80 */
/* Address: 0x8013EE9C */
/* Size: 80 bytes (20 instructions) */
#include "types.h"
#include "tokimeki/sprite.h"

extern u8 bustup_cw_disp;
extern u8 bustup2_cw_disp;
extern u8 cal_cw_disp;
extern s32 Sprites;

void etc_sprite_clear(void)
{
    bustup2_cw_disp = 0;
    bustup_cw_disp = 0;
    cal_cw_disp = 0;
    Sprites = (s32)0x80000000;
    (*(s32 *)&((SPRITE_REC *)&Sprites)[1].attribute) = (s32)0x80000000;
    (*(s32 *)&((SPRITE_REC *)&Sprites)[2].attribute) = (s32)0x80000000;
    (*(s32 *)&((SPRITE_REC *)&Sprites)[3].attribute) = (s32)0x80000000;
    (*(s32 *)&((SPRITE_REC *)&Sprites)[4].attribute) = (s32)0x80000000;
    (*(s32 *)&((SPRITE_REC *)&Sprites)[5].attribute) = (s32)0x80000000;
}
