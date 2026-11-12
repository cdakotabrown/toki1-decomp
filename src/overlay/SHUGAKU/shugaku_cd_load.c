/* @SymbolStart: 0x8013A6F0 */
/* @SymbolSize: 96 */
#include "types.h"

extern u8 shugaku_place;

extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void shugaku_buf_ptr_init(void);
extern void sub_sub_step_up(void);

void shugaku_cd_load(void)
{
    s32 param_t6 = shugaku_place;

    if (param_t6 != 1) goto lbl_8013A730;
    cd_read_start2(61, 0x801E0000, 0xBD12);
    shugaku_buf_ptr_init();
    sub_sub_step_up();
    goto lbl_8013A744;
lbl_8013A730:
    sub_sub_step_up();
    sub_sub_step_up();
lbl_8013A744:
    return;
}
