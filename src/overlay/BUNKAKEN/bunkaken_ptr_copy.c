/* Address: 0x8013BFA0 */
/* Size: 52 bytes (13 insns; the .sym entry is 64 = +3 alignment nops) */
/* @SymbolStart: 0x8013BFA0 */
/* @SymbolSize: 52 */
#include "types.h"

extern s32 bunkaken_bank_ptr[];
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;

void bunkaken_ptr_copy(void) {
    ss_serifu = bunkaken_bank_ptr[0];
    ss_serifu_kind = bunkaken_bank_ptr[1];
    sss_sound = bunkaken_bank_ptr[2];
}
