/* @SymbolStart: 0x8013F9F8 */
/* @SymbolSize: 100 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=128 */
#include "types.h"
#include "tokimeki/sprite.h"

extern void sub_sub_step_up(void);

extern s32 bustup_on;
extern u8 bustup2_cw_bright;
extern SPRITE_REC Sprites[];

void date_basyo18_bright_init(void)
{
    bustup_on = 1;
    Sprites[0].b = 128;
    Sprites[0].g = 128;
    Sprites[0].r = 128;
    Sprites[1].b = 128;
    Sprites[1].g = 128;
    Sprites[1].r = 128;
    bustup2_cw_bright = 128;
    sub_sub_step_up();
}
