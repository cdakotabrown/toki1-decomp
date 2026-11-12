/* Address: 0x801321D4 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_girl3_cl_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801EA570;
    bunkaken_bank_ptr[1] = 0x801EA834;
    bunkaken_bank_ptr[2] = 0x801EC1DC;
}
