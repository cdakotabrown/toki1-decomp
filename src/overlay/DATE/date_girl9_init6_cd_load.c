/* @SymbolSize: 56 */
/* Function: date_girl9_init6_cd_load */
/* Overlay: DATE */
/* Address: 0x801585FC */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern s32 cd_read_start2();
extern void date_ptrtbl16_init();
extern s32 sub_sub_step_up();

void date_girl9_init6_cd_load(void) {
    cd_read_start2(61, 0x801B0000, 0x8B8C);
    date_ptrtbl16_init();
    sub_sub_step_up();
}
