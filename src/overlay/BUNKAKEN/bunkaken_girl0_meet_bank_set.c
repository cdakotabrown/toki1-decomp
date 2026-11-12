/* Address: 0x80132000 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_girl0_meet_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801E8A20;
    bunkaken_bank_ptr[1] = 0x801E8BC8;
    bunkaken_bank_ptr[2] = 0x801E92F4;
}
