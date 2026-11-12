/* Address: 0x8013BDE0 */
/* Size: 76 bytes (19 instructions) */
/* @SymbolSize: 76 */
#include "types.h"

extern void sub_sub_step_up();
extern void date_name_set();
extern void event_log_add();
extern u8 now_girl;
extern s32 serifu_no;

void shougatu_girl_taku_set2(void)
{
    s32 t6;

    t6 = serifu_no;
    now_girl = t6;
    event_log_add(7, 0);
    date_name_set(now_girl);
    sub_sub_step_up();
}
