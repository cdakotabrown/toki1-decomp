/* Address: 0x8013A33C */
/* Size: 24 bytes (6 insns) */
#include "types.h"

extern u8 option_main_vab_wk;

void option_main_vab_step(void) {
    option_main_vab_wk = option_main_vab_wk + 1;
}
