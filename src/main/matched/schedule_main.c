/* Address: 0x80070FD4 */
/* Size: 108 bytes (27 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack 24 40 */
/* @PostProc: strip_trailing_instructions 1 */
#include "types.h"

extern u8 now_sub_step;

extern void _schedule_main(void);
extern void parameter_show(void);
extern void message_window_show(void);
extern void hizuke_show(void);
extern void icon_cursor_move(s32);
extern void mascot_move(void);
extern s32  get_last_gamen_mode(void);
extern void dec_bg_show(s32);
extern void update_anim_state(s32);

void schedule_main(void) {
    if (now_sub_step == 0) {
        _schedule_main();
    }
    parameter_show();
    message_window_show();
    hizuke_show();
    icon_cursor_move(0);
    mascot_move();
    dec_bg_show(get_last_gamen_mode());
    update_anim_state(1);
}
