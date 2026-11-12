/* Address: 0x801386DC, Size: 64 bytes (16 instructions) */
#include "types.h"
extern void class_clear_all();
extern void goto_step();
extern void option_menu_box_show();
extern u8 save_buf;
extern u8 now_girl;

void option_exit10(void) {
    class_clear_all(0);
    now_girl = save_buf;
    goto_step(145);
    option_menu_box_show();
}
