/* Address: 0x80042218 */
/* Size: 120 bytes (30 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 now_girl;
extern u8 k_cnt;
extern u8 k_no;
extern u8 k_mode;
extern u8 k_aux;
extern u8 icon_mode;
extern u8 voice_level;
extern u8 click_mode;
extern u8 click_cnt;
extern u8 click_speed;
extern u8 mouse_btn_mode;
extern u8 icon_num;
extern u8 bustup_sw;

void game_work_init2(void) {
    now_girl = 0;
    k_cnt = 0;
    k_no = 0;
    k_mode = 0;
    k_aux = 0;
    icon_mode = 0;
    voice_level = 2;
    click_mode = 0;
    click_cnt = 0;
    click_speed = 32;
    mouse_btn_mode = 0;
    icon_num = 14;
    bustup_sw = 0;
}
