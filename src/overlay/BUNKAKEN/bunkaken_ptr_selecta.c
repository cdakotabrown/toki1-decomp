/* Address: 0x80132478 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_ptr_selecta(void) {
    bunkaken_bank_ptr[0] = 0x801E840C;
    bunkaken_bank_ptr[1] = 0x801E845C;
    bunkaken_bank_ptr[2] = 0x801E87A8;
}
