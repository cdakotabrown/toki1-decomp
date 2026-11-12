/* Address: 0x8006CB84, Size: 68 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern u8 pad_type;
extern u8 mouse_btn_mode;
extern u8 get_pad_str2_tbl0;
extern u8 get_pad_str2_tbl1;
extern u8 get_pad_str2_tbl2;
void *get_pad_str2(void) {
    if (pad_type == 0) {
        if (mouse_btn_mode != 0) {
            return &get_pad_str2_tbl2;
        }
        return &get_pad_str2_tbl1;
    }
    return &get_pad_str2_tbl0;
}
