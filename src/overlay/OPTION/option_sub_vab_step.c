/* Address: 0x8013A324 */
/* Size: 24 bytes (6 insns) */
#include "types.h"

extern u8 option_sub_vab_wk;

void option_sub_vab_step(void) {
    option_sub_vab_wk = option_sub_vab_wk + 1;
}
