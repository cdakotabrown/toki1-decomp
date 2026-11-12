/* Address: 0x80132548 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_fort_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801E81CC;
    bunkaken_bank_ptr[1] = 0x801E8204;
    bunkaken_bank_ptr[2] = 0x801E82E4;
}
