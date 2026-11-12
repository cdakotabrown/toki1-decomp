/* Address: 0x8013209C */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_girl0_pair_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801EBDEC;
    bunkaken_bank_ptr[1] = 0x801EC320;
    bunkaken_bank_ptr[2] = 0x801EF54C;
}
