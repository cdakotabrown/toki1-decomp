/* Function: date_girl6_mode5_res_load */
/* Overlay: DATE */
/* Address: 0x80157754 */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern s32 cd_read_start2();
extern void date_girl6_mode5_bufptr_init();
extern s32 sub_sub_step_up();

void date_girl6_mode5_res_load(void) {
    cd_read_start2(61, 0x801B0000, 0x8A4B);
    date_girl6_mode5_bufptr_init();
    sub_sub_step_up();
}
