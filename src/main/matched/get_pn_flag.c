/* Address: 0x8004B144 */
/* Size: 16 bytes (4 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 _debug_number_show_tbl0;

u8 get_pn_flag(void) {
    return _debug_number_show_tbl0;
}
