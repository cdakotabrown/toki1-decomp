/* Address: 0x8013223C */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_girl4_pair_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801EB18C;
    bunkaken_bank_ptr[1] = 0x801EB518;
    bunkaken_bank_ptr[2] = 0x801ED920;
}
