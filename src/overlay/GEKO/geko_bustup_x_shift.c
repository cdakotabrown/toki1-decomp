/* @SymbolStart: 0x80141D0C */
/* @SymbolSize: 108 */
#include "types.h"
#include "tokimeki/sprite.h"
#include "tokimeki/class_w.h"

extern void sub_sub_step_up(void);
extern CLASS_W class_w[];
extern SPRITE_REC Sprites[];

void geko_bustup_x_shift(void)
{
    (&class_w[30].x.half[1])[34] = (&class_w[30].x.half[1])[34] + 64;
    (&class_w[30].x.half[1])[0] = (&class_w[30].x.half[1])[0] + 64;
    (&Sprites[0].x)[0] = (&Sprites[0].x)[0] + 64;
    (&Sprites[0].x)[18] = (&Sprites[0].x)[18] + 64;
    sub_sub_step_up();
}
