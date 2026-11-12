/* @SymbolStart: 0x80132B34 */
/* @SymbolSize: 120 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 */
/* Address: 0x80132B3C, Size: 120 bytes (30 instructions) */
#include "types.h"

extern void option_init0();
extern void option_init1();
extern void option_cursor2();
extern void game_reset();
extern u8 now_sub_sub_step;

void option_top_menu_step(void) {
    s32 hidden_v1;

    hidden_v1 = now_sub_sub_step;
    switch (hidden_v1) {
        case 0:
            option_init0();
            break;
        case 1:
            option_init1();
            break;
        case 2:
            option_cursor2();
            break;
        default:
            game_reset();
            break;
    }
}
