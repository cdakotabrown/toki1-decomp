/* Address: 0x8013216C */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_girl2_cl_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801EA384;
    bunkaken_bank_ptr[1] = 0x801EA614;
    bunkaken_bank_ptr[2] = 0x801EBFF8;
}
