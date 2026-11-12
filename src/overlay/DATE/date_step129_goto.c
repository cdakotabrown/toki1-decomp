/* @SymbolSize: 64 */
/* Function: date_step129_goto */
/* Overlay: DATE */
#include "types.h"

extern s32 goto_step();
extern s32 class_clear_all();
extern s32 set_flag_b3_high_bit();

extern u8 girl_no;
extern u8 now_girl;

void date_step129_goto(void)
{
    u8 temp;

    class_clear_all(0);
    set_flag_b3_high_bit(0);
    temp = girl_no;
    now_girl = temp;
    goto_step(129);
}
