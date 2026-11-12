/* Address: 0x80043688 */
/* Size: 200 bytes (50 instructions; 47-word body after the types.h thunks) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 date2_menu_select_tbl1;
extern u8 etc_init_tbl0;
extern u8 date2_menu_select_tbl0;
extern s32 konami_command_check_tbl0[];
extern s32 pad_data;

void konami_command_check(void) {
    if (date2_menu_select_tbl1 != 0) {
        etc_init_tbl0++;
        if (etc_init_tbl0 == 0) {
            date2_menu_select_tbl1 = 0;
            date2_menu_select_tbl0 = 0;
        }
        return;
    }
    if (konami_command_check_tbl0[date2_menu_select_tbl0] == pad_data) {
        date2_menu_select_tbl0++;
        if (date2_menu_select_tbl0 >= 18U) {
            date2_menu_select_tbl1 = 1;
        }
        etc_init_tbl0 = 16;
    }
    etc_init_tbl0--;
    if (etc_init_tbl0 == 0) {
        date2_menu_select_tbl0 = 0;
    }
}
