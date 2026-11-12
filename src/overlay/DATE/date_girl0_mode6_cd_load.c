/* @SymbolSize: 56 */
/* Function: date_girl0_mode6_cd_load */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 cd_read_start2();
extern void date_girl0_mode6_bg_ptr_init();

void date_girl0_mode6_cd_load(void)
{
    cd_read_start2(53, 0x801B0000, 0x8030);
    date_girl0_mode6_bg_ptr_init();
    sub_sub_step_up();
}
