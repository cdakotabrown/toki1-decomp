/* Address: 0x80132104 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_girl1_cl_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801E9C78;
    bunkaken_bank_ptr[1] = 0x801E9E44;
    bunkaken_bank_ptr[2] = 0x801EB0AC;
}
