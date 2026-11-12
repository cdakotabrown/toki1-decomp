/* Function: pre_syogatu_main */
/* Address: 0x800618B8 */
/* Size: 128 bytes (32 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0 v1 */
#include "types.h"

extern u8 now_sub_step;
extern void pre_syogatu_init(void);
extern void pre_syogatu(void);
extern void hizuke_show(void);
extern void message_window_show(void);
extern void step_directional_scroll_once(void);
extern void basyo_show(void);
extern void dec_bg_show(s32);
extern void update_anim_state(s32);

void pre_syogatu_main(void) {
    s32 mode = now_sub_step;
    switch (mode) {
    case 0:
        pre_syogatu_init();
        break;
    case 1:
        pre_syogatu();
        break;
    }
    hizuke_show();
    message_window_show();
    step_directional_scroll_once();
    basyo_show();
    dec_bg_show(1);
    update_anim_state(1);
}
