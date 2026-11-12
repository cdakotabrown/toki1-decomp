/* date2_song_res_load - DATE2 overlay, Address: 0x801381D4, Size: 56 bytes (14 insns) */
/* @SymbolSize: 56 */
#include "types.h"

void cd_read_start2(s32 arg0, void *arg1, s32 arg2);
void date2_ptr_tbl_init2(void);
void sub_sub_step_up(void);

void date2_song_res_load(void)
{
    cd_read_start2(3, (void *)0x801B0000, 0xAF39);
    date2_ptr_tbl_init2();
    sub_sub_step_up();
}
