/* Address: 0x80134E9C (true start 0x80134E90, .sym late by 3 insns) */
/* @SymbolStart: 0x80134E90 */
/* @SymbolSize: 68 */
#include "types.h"

extern void date_name_set(s32);
extern void sub_sub_step_up(void);

extern u8 now_girl;
extern u8 girl_no;

void shugaku_girl13_set(void)
{
    girl_no = now_girl;
    now_girl = 13;
    date_name_set(13);
    sub_sub_step_up();
}
