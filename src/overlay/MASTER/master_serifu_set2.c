/* @SymbolSize: 52 */
/* Address: 0x80133680 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 master_serifu_ptr[];

void master_serifu_set2(void) {
    master_serifu_ptr[0] = 2149484128;
    master_serifu_ptr[1] = 2149484352;
    master_serifu_ptr[2] = 2149485304;
}
