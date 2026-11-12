/* @SymbolStart: 0x801349DC */
/* @SymbolSize: 52 */
/* shugaku_girl_restore2 - SHUGAKU overlay, Address: 0x801349DC, Size: 52 bytes (13 insns) */
#include "types.h"

extern u8 now_girl;
extern u8 girl_no;

extern void date_name_set(s32);
extern void sub_sub_step_up(void);

void shugaku_girl_restore2(void)
{
    s32 a0 = girl_no;

    now_girl = (u8)a0;
    date_name_set(a0);
    sub_sub_step_up();
}
