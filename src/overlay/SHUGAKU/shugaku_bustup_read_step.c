/* @SymbolStart: 0x801396A0 */
/* @SymbolSize: 76 */
#include "types.h"

extern u8 date_place;

extern void bustup_cd_read(s32 a0);
extern void sub_sub_step_up(void);

void shugaku_bustup_read_step(void)
{
    s32 param_t6 = date_place;

    if (param_t6 == 0) goto lbl_801396C4;
    bustup_cd_read(27698);
    goto lbl_801396D4;
lbl_801396C4:
    sub_sub_step_up();
    sub_sub_step_up();
lbl_801396D4:
    sub_sub_step_up();
}
