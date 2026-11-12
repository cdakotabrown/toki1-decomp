/* @SymbolStart: 0x8014A1F0 */
/* @SymbolSize: 44 */
/* Function: date_open_load_step */
/* Overlay: DATE */
/* Address: 0x8014A1F0 (cross-prologue head) / 0x8014A1F8 (body) */
/* Size: 44 bytes (11 instructions) */
#include "types.h"

extern s16 date_open_sub;

extern void date_open_load();

void date_open_load_step(void) {
    if (date_open_sub == 0) {
        date_open_load();
    }
}
