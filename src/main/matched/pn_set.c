/* Address: 0x8004B11C */
/* Size: 40 bytes (10 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 _debug_number_show_tbl0;

void pn_set(u8 arg0) {
    _debug_number_show_tbl0 = arg0;
    if (arg0 >= 9U) {
        _debug_number_show_tbl0 = 8;
    }
}
