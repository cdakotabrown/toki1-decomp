/* Function: byouki_main */
/* Address: 0x80072C80 */
/* Size: 168 bytes (42 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0:v1 --exclude 0x94 */
#include "types.h"

extern u8 now_sub_step;
extern void byouki_init(void);
extern void byouki_sub_step_main(void);
extern void byouki_disp(void);
extern void parameter_show(void);
extern void message_window_show(void);
extern void hizuke_show(void);
extern void icon_cursor_move(s32);
extern void mascot_move(void);
extern void update_anim_state(s32);
extern s32 get_last_gamen_mode(void);
extern void dec_bg_show(s32);

void byouki_main(void) {
    s32 mode = now_sub_step;
    switch (mode) {
    case 0:
        byouki_init();
        break;
    case 1:
        byouki_sub_step_main();
        break;
    case 255:
    default:
        byouki_disp();
        break;
    }
    parameter_show();
    message_window_show();
    hizuke_show();
    icon_cursor_move(0);
    mascot_move();
    update_anim_state(1);
    dec_bg_show(get_last_gamen_mode());
}
