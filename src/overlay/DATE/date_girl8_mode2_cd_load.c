/* @SymbolSize: 56 */
/* Function: date_girl8_mode2_cd_load */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 cd_read_start2();
extern s32 date_bufptr7dd3_init();

void date_girl8_mode2_cd_load(void)
{
    cd_read_start2(61, 0x801B0000, 32211);
    date_bufptr7dd3_init();
    sub_sub_step_up();
}
