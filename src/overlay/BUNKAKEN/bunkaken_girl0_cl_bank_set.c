/* Address: 0x80132034 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_girl0_cl_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801EE270;
    bunkaken_bank_ptr[1] = 0x801EE93C;
    bunkaken_bank_ptr[2] = 0x801F2DA4;
}
