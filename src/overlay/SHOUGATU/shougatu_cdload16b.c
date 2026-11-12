/* Address: 0x80137A54 */
/* Size: 88 bytes (22 instructions) */
/* @SymbolSize: 88 */
#include "types.h"

extern void sub_sub_step_up();
extern void cd_read_start2();
extern void shougatu_ptab_init();
extern u8 schedule_kind;
extern u8 schedule_cursor;
extern u8 shougatu_date_girl;

void shougatu_cdload16b(void)
{
    u8 *ptr;
    cd_read_start2(16, (void*)0x80197000, 0xAEFD);
    shougatu_ptab_init();
    ptr = &schedule_kind + (schedule_cursor << 2);
    shougatu_date_girl = *ptr;
    sub_sub_step_up();
}
