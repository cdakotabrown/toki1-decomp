/* Function: date_girl9_init4_cd_load */
/* Overlay: DATE */
/* Address: 0x801583D4 */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern s32 cd_read_start2();
extern void date_ptrtbl14_init();
extern s32 sub_sub_step_up();

void date_girl9_init4_cd_load(void) {
    cd_read_start2(69, 0x801B0000, 0x8B0A);
    date_ptrtbl14_init();
    sub_sub_step_up();
}
