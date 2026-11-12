/* @SymbolSize: 52 */
/* rensyu_serifu_tbl_set - RENSYU overlay, Address: 0x80132000, Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 rensyu_serifu_ptr;
extern s32 rensyu_serifu_kind_ptr;
extern s32 rensyu_sound_ptr;

void rensyu_serifu_tbl_set(void) {
    rensyu_serifu_ptr = 0x801E990C;
    rensyu_serifu_kind_ptr = 0x801E9C40;
    rensyu_sound_ptr = 0x801EB2D8;
}
