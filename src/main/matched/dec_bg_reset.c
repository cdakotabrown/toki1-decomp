/* Address: 0x80057714 */
/* Size: 76 bytes (19 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

s32 date2_intro_script_tbl0[4] = { 0, 0, 0, 0 };
extern s32 dec_bg_reset_flag;
u8  bg_change_tbl0[2] = { 0, 0 };
extern u8  dec_bg_show_tbl0;
extern s32 cd_wait_timeout_tbl0;
extern u8  fade_bri;

void dec_bg_reset(void) {
    date2_intro_script_tbl0[3] = 0;
    date2_intro_script_tbl0[1] = 0;
    date2_intro_script_tbl0[2] = 0;
    date2_intro_script_tbl0[0] = 0;
    bg_change_tbl0[1] = 0;
    bg_change_tbl0[0] = 0;
    fade_bri = 128;
    cd_wait_timeout_tbl0 = -1;
    dec_bg_reset_flag = 0;
    dec_bg_show_tbl0 = 0;
}
