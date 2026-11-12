/* Address: 0x80132444 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_girl9_pair_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801EA150;
    bunkaken_bank_ptr[1] = 0x801EA3E0;
    bunkaken_bank_ptr[2] = 0x801EC088;
}
