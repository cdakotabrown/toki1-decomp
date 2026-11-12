/* Address: 0x80132410 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 bunkaken_bank_ptr[];

void bunkaken_girl9_meet_bank_set(void) {
    bunkaken_bank_ptr[0] = 0x801E838C;
    bunkaken_bank_ptr[1] = 0x801E83E4;
    bunkaken_bank_ptr[2] = 0x801E86FC;
}
