/* Address: 0x8004EB80 */
/* Size: 48 bytes (12 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 k_cell_w;

void kanji_cell_width_set(u8 arg0) {
    if (arg0 == 16) {
        k_cell_w = 16;
    } else {
        k_cell_w = 14;
    }
}
