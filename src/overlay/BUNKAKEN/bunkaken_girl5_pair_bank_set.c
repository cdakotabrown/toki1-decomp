/* Address: 0x801322A4 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_girl5_pair_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801EC9D4;
    bunkaken_bank_ptr[1] = 0x801ECE08;
    bunkaken_bank_ptr[2] = 0x801EFD50;
}
