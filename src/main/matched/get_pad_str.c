/* Address: 0x8006CB40 */
/* Size: 68 bytes (17 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 pad_type;
extern u8 mouse_btn_mode;
extern u8 get_pad_str_tbl0[];
extern u8 get_pad_str2_tbl1[];
extern u8 get_pad_str2_tbl2[];

u8 *get_pad_str(void) {
    u8 *p = get_pad_str_tbl0;

    if (pad_type == 0) {
        if (mouse_btn_mode != 0) {
            return get_pad_str2_tbl1;
        }
        return get_pad_str2_tbl2;
    }
    return p;
}
