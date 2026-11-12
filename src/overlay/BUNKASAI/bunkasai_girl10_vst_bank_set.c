/* Address: 0x80132374 */
/* Size: 52 bytes (13 insns) */
/* @SymbolSize: 52 */
#include "types.h"

extern s32 bunkasai_serifu_ptr[];

void bunkasai_girl10_vst_bank_set(void) {
    bunkasai_serifu_ptr[0] = 0x801ED184;
    bunkasai_serifu_ptr[1] = 0x801ED664;
    bunkasai_serifu_ptr[2] = 0x801F0F78;
}
