/* Address: 0x801324E0 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_common_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801E92FC;
    bunkaken_bank_ptr[1] = 0x801E9598;
    bunkaken_bank_ptr[2] = 0x801EA298;
}
