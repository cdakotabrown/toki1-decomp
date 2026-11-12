/* @SymbolSize: 52 */
/* Address: 0x801336e8 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 master_serifu_ptr[];

void master_serifu_set(void) {
    master_serifu_ptr[0] = 2149485072;
    master_serifu_ptr[1] = 2149485436;
    master_serifu_ptr[2] = 2149487572;
}
