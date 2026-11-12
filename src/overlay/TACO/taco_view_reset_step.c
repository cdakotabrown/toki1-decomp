/* @SymbolStart: 0x80144948 */
/* @SymbolSize: 64 */
#include "types.h"
#include "tokimeki/libgs_types.h"
extern LIBGS_DOBJ2 taco_obj[];

extern void draw2d3d(s32, s32);
extern void goto_sub_step(s32);

void taco_view_reset_step(void)
{
    (*(s32 *)&taco_obj[1].attribute) = (s32)0x80000000;
    (*(s32 *)&taco_obj[2].attribute) = (s32)0x80000000;
    draw2d3d(1, 0);
    goto_sub_step(2);
}
