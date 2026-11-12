/* Address: 0x80078A70, Size: 44 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern u8  overlay_load_disp_tbl0;
extern u8  overlay_load_main_tbl0;
extern u8  ending_main_tbl0;
extern s32 ending_main_tbl1;
extern s32 overlay_load_init_tbl0;
void overlay_work_clear(void) {
    overlay_load_disp_tbl0 = 0;
    overlay_load_main_tbl0 = 0;
    ending_main_tbl0 = 0;
    ending_main_tbl1 = 0;
    overlay_load_init_tbl0 = 0;
}
