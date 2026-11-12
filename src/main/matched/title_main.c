/* Address: 0x80062A0C */
/* Size: 224 bytes (56 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0 v1 */
#include "types.h"

extern s32 step_state;
extern u8 now_sub_step;
extern u8 date2_menu_select_tbl1;
extern void title_data_read(void);
extern void title_move(void);
extern void title_set(void);
extern void title_main3(void);
extern void game_reset(void);
extern void set_flag_b3_high_bit(s32);
extern void device_cmd_send(s32);
extern void goto_step(s32);
extern void goto_sub_step(s32);

void title_main(void) {
    u8 mode;

    step_state = step_state + 1;
    mode = now_sub_step;

    switch (mode) {
    case 0:
        title_data_read();
        break;
    case 1:
        title_move();
        break;
    case 2:
        title_set();
        break;
    case 3:
        title_main3();
        break;
    default:
        game_reset();
        break;
    }

    if (date2_menu_select_tbl1 != 0) {
        set_flag_b3_high_bit(0);
        device_cmd_send(127);
        goto_step(17);
        goto_sub_step(1);
        device_cmd_send(1281);
    }
}
