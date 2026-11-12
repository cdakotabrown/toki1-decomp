/* Address: 0x80132208 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_girl4_meet_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801E83F4;
    bunkaken_bank_ptr[1] = 0x801E8458;
    bunkaken_bank_ptr[2] = 0x801E87C8;
}
