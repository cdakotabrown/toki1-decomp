/* Function: date_bustup31_step */
/* Overlay: DATE */
/* Address: 0x8014EE08 */
/* Size: 36 bytes (9 instructions) */
#include "types.h"

extern u8 bustup_cd_read_tbl0;
extern void bustup_modes31_step();

void date_bustup31_step(void) {
    bustup_modes31_step();
    bustup_cd_read_tbl0 = 0;
}
