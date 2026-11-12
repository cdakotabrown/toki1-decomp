/* Address: 0x80135C08 (true start 0x80135C04, .sym late by 1 insn) */
/* @SymbolStart: 0x80135C04 */
/* @SymbolSize: 48 */
#include "types.h"

extern void sub_sub_step_up(void);

extern s8 shugaku_float_on;
extern u8 update_anim_state_tbl4;

void shugaku_float_exit(void)
{
    shugaku_float_on = 0;
    update_anim_state_tbl4 = 15;
    sub_sub_step_up();
}
