/* @SymbolSize: 56 */
/* Function: date_player_birthday_cd_load */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 cd_read_start2();
extern void date_load3_ptr_set();

void date_player_birthday_cd_load(void)
{
    cd_read_start2(3, 0x801B0000, 0xAF43);
    date_load3_ptr_set();
    sub_sub_step_up();
}
