/* Address: 0x801323DC */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_girl8_pair_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801EC430;
    bunkaken_bank_ptr[1] = 0x801EC8BC;
    bunkaken_bank_ptr[2] = 0x801EF910;
}
