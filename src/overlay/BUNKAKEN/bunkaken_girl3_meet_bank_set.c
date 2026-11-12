/* Address: 0x801321A0 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_girl3_meet_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801E862C;
    bunkaken_bank_ptr[1] = 0x801E8724;
    bunkaken_bank_ptr[2] = 0x801E8BC8;
}
