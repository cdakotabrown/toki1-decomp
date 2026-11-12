/* Address: 0x801324AC */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_girl10_pair_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801EB950;
    bunkaken_bank_ptr[1] = 0x801EBC94;
    bunkaken_bank_ptr[2] = 0x801EE3D0;
}
