/* Address: 0x801320D0 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_girl1_meet_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801E8630;
    bunkaken_bank_ptr[1] = 0x801E872C;
    bunkaken_bank_ptr[2] = 0x801E8BEC;
}
