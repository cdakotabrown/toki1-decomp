/* Function: date_basyo1_spring_cd_load */
/* Overlay: DATE */
/* Address: 0x801402C8 */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern s32 cd_read_start2();
extern void date_buf_tbl2_init();
extern s32 sub_sub_step_up();

void date_basyo1_spring_cd_load(void) {
    cd_read_start2(34, 0x801E0000, 0xBC21);
    date_buf_tbl2_init();
    sub_sub_step_up();
}
