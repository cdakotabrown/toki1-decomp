/* Function: date_girl_birthday_cd_load */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 cd_read_start2();
extern void date_ptrtab_init();

void date_girl_birthday_cd_load(void)
{
    cd_read_start2(22, 0x801B0000, 0xAF0D);
    date_ptrtab_init();
    sub_sub_step_up();
}
