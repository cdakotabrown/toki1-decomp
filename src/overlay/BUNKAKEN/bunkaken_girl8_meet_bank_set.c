/* Address: 0x801323A8 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_girl8_meet_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801E8564;
    bunkaken_bank_ptr[1] = 0x801E85F4;
    bunkaken_bank_ptr[2] = 0x801E89D0;
}
