/* @SymbolStart: 0x8013C56C */
/* @SymbolSize: 48 */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 bustup_mode;
extern u8 cal_cw_mode;

void geko_bustup_mode_set(void)
{
    bustup_mode = 2;
    cal_cw_mode = 0;
    sub_sub_step_up();
}
