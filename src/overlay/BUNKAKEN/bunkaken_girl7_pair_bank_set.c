/* Address: 0x80132374 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_girl7_pair_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801ED8DC;
    bunkaken_bank_ptr[1] = 0x801EDD8C;
    bunkaken_bank_ptr[2] = 0x801F15D8;
}
