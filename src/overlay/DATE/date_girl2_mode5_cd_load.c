/* @SymbolSize: 56 */
/* Function: date_girl2_mode5_cd_load */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 cd_read_start2();
extern s32 date_ptrtbl4_init();

void date_girl2_mode5_cd_load(void)
{
    cd_read_start2(69, 0x801B0000, 0x81FB);
    date_ptrtbl4_init();
    sub_sub_step_up();
}
