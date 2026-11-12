/* @SymbolStart: 0x80135D98 */
/* @SymbolSize: 104 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 */
/* Address: 0x80135DA0, Size: 104 bytes (26 instructions) */
#include "types.h"

extern void option_menu_mode_init(void);
extern void option_menu_mode_main(void);
extern void option_zoom_trans(void);
extern u8 now_sub_sub_step;

void option_menu_mode_step(void) {
    s32 a0;

    a0 = now_sub_sub_step;
    switch (a0) {
    case 0:
        option_menu_mode_init();
        break;
    case 1:
        option_menu_mode_main();
        break;
    case 2:
        option_zoom_trans();
        break;
    }
}
