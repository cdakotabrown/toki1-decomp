/* Address: 0x80041A00 */
/* Size: 56 bytes (14 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8  pad_type;
extern u8  csr_speed;
extern u8  menu_mode;
extern u32 game_mode_step;

void game_system_init(void) {
    pad_type = 1;
    csr_speed = 3;
    menu_mode = 1;
    game_mode_step = 0x80132000;
}
