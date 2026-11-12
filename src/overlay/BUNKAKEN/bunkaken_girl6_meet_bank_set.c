/* Address: 0x801322D8 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_girl6_meet_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801E8308;
    bunkaken_bank_ptr[1] = 0x801E8350;
    bunkaken_bank_ptr[2] = 0x801E85F4;
}
