/* Function: _schedule_init */
/* Address: 0x8006CC0C */
/* Size: 188 bytes (47 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 now_sub_sub_step;
extern void schedule_init0(void);
extern void schedule_make(void);
extern void schedule_girls_gakko(void);
extern void schdeule_girls_param(void);
extern void schedule_init4(void);
extern void parameter_show(void);
extern void message_window_show(void);
extern void hizuke_show(void);
extern void icon_cursor_move(s32);
extern void mascot_move(void);
extern void update_anim_state(s32);

void _schedule_init(void) {
    s32 mode = now_sub_sub_step;
    switch (mode) {
    case 0:
        schedule_init0();
        break;
    case 1:
        schedule_make();
        break;
    case 2:
        schedule_girls_gakko();
        break;
    case 3:
        schdeule_girls_param();
        break;
    case 4:
        schedule_init4();
        break;
    }
    parameter_show();
    message_window_show();
    hizuke_show();
    icon_cursor_move(0);
    mascot_move();
    update_anim_state(1);
}
