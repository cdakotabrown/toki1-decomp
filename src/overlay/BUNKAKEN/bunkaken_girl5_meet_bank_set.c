/* Address: 0x80132270 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_girl5_meet_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801E8408;
    bunkaken_bank_ptr[1] = 0x801E845C;
    bunkaken_bank_ptr[2] = 0x801E8754;
}
