/* Address: 0x8013257C */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_birth_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801E8A10;
    bunkaken_bank_ptr[1] = 0x801E8B7C;
    bunkaken_bank_ptr[2] = 0x801E93D4;
}
