/* Function: pre_xmas_main */
/* Address: 0x80061030 */
/* Size: 168 bytes (42 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0 v1 */
#include "types.h"

extern u8 now_sub_step;
extern u8 mascot_cw_disp;
extern void pre_xmas_init(void);
extern void pre_xmas(void);
extern void parameter_show(void);
extern void hizuke_show(void);
extern void message_window_show(void);
extern void step_directional_scroll_once(void);
extern void basyo_show(void);
extern void mascot_move(void);
extern void dec_bg_show(s32);
extern void update_anim_state(s32);

void pre_xmas_main(void) {
    s32 mode = now_sub_step;
    switch (mode) {
    case 0:
        pre_xmas_init();
        break;
    case 1:
        pre_xmas();
        break;
    }
    parameter_show();
    hizuke_show();
    message_window_show();
    step_directional_scroll_once();
    basyo_show();
    if (mascot_cw_disp & 0x80) {
        mascot_move();
    }
    dec_bg_show(0);
    update_anim_state(1);
}
