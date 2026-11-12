/* Address: 0x800738B4 */
/* Size: 224 bytes (56 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack 24 40 */
/* @PostProc: patch_insn_reg_field 0x0:rt:v0:v1 0x4:rt:v0:v1 0x4:rs:v0:v1 0xc:rs:v0:v1 0x18:rs:v0:v1 0x20:rs:v0:v1 0x28:rs:v0:v1 */
#include "types.h"

extern u8 now_sub_step;
extern u8 mascot_cw_disp;

extern void week_day_init(void);
extern void week_day_main(void);
extern void week_day_exit(void);
extern void vacation_day_init(void);
extern void game_reset(void);

extern void parameter_show(void);
extern void message_window_show(void);
extern void hizuke_show(void);
extern void icon_cursor_move(s32);
extern void dec_bg_show(s32);
extern void update_anim_state(s32);
extern void mascot_move(void);

void week_day(void) {
    switch (now_sub_step) {
    case 0:
        week_day_init();
        break;
    case 1:
        week_day_main();
        break;
    case 2:
        week_day_exit();
        break;
    case 10:
        vacation_day_init();
        break;
    default:
        game_reset();
        break;
    }

    parameter_show();
    message_window_show();
    hizuke_show();
    icon_cursor_move(1);
    dec_bg_show(0);
    update_anim_state(1);

    if (mascot_cw_disp & 0x80) {
        mascot_move();
    }
}
