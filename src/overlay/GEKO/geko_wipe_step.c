/* @SymbolStart: 0x801377F8 */
/* @SymbolSize: 80 */
/* geko_wipe_step -- GEKO overlay, Address: 0x80137800, Size: 72 bytes (18 insns) */
#include "types.h"

extern void wipe_main(void);
extern void sub_sub_step_up(void);

extern s8 geko_wipe_flag;
extern u8 geko_girl_flag;

void geko_wipe_step(void)
{
    s32 t6 = geko_girl_flag;

    if (t6 != 1) goto else_branch;
    geko_wipe_flag = 1;
    wipe_main();
    return;

else_branch:
    sub_sub_step_up();
    sub_sub_step_up();
}
