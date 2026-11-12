/* Size: 64 bytes */
/* @SymbolSize: 64 */
#include "types.h"

extern s32 shougatu_menu_bg_res[];

void shougatu_ptr_set2(void) {
    shougatu_menu_bg_res[0] = 0x801B21E0;
    shougatu_menu_bg_res[1] = 0x801B21E4;
    shougatu_menu_bg_res[2] = 0x801B2228;
    shougatu_menu_bg_res[4] = 0x801B0000;
}
