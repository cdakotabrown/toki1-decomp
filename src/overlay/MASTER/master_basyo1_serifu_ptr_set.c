/* @SymbolSize: 52 */
#include "types.h"

extern s32 master_basyo_serifu_ptr[];

void master_basyo1_serifu_ptr_set(void) {
    master_basyo_serifu_ptr[0] = 0x801E8A1C;
    master_basyo_serifu_ptr[1] = 0x801E8B58;
    master_basyo_serifu_ptr[2] = 0x801E94B8;
}
