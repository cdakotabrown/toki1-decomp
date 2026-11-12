/* Address: 0x80132068 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_ptr_selectb(void) {
    bunkaken_bank_ptr[0] = 0x801E829C;
    bunkaken_bank_ptr[1] = 0x801E82E8;
    bunkaken_bank_ptr[2] = 0x801E8538;
}
