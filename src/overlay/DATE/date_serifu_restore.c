/* @SymbolStart: 0x80149608 */
/* @SymbolSize: 44 */
/* Function: date_serifu_restore */
/* Overlay: DATE */
/* Address: 0x80149608 (cross-prologue head) / 0x80149610 (body) */
/* Size: 44 bytes (11 instructions) */
#include "types.h"

extern u8 date_serifu_no_save;
extern s32 serifu_no;

extern s32 sub_sub_step_up();

s32 date_serifu_restore(void) {
    serifu_no = date_serifu_no_save;
    return sub_sub_step_up();
}
