/* @SymbolStart: 0x80139700 */
/* @SymbolSize: 336 */
/* @RodataBase: 0x8013FDF0 */
/* tel_step_dispatch - TEL overlay, Address: 0x8013971C, Size: 308 bytes (77 insns) */
#include "types.h"

typedef struct {
    s32 count;
    u8  unk4;
    u8  step;
    u8  sel;
} StepState;

extern StepState step_counter;
extern u8 mascot_cw_disp;

extern void tel_rusuban_basyo_miss_main(void);
extern void tel_rusuban_date_forget_main(void);
extern void tel_yoshio_reserved_main(void);
extern void tel_tatebayasi_main(void);
extern void tel_home_ring_dial_main(void);
extern void tel_tome_main(void);
extern void tel_call_girl_main(void);
extern void tel_call_girl_dialog2_main(void);
extern void tel_basyo_main(void);
extern void tel_sub_sub_dispatch(void);
extern void hizuke_show(void);
extern void message_window_show(void);
extern void parameter_show(void);
extern void icon_cursor_move(s32);
extern void step_directional_scroll_once(void);
extern void basyo_show(void);
extern void dec_bg_show(s32);
extern void update_anim_state(s32);
extern void mascot_move(void);

void tel_step_dispatch(void) {
    step_counter.count++;
    switch (step_counter.step) {
        case 0:
            tel_rusuban_basyo_miss_main();
            break;
        case 1:
            tel_rusuban_date_forget_main();
            break;
        case 2:
            tel_yoshio_reserved_main();
            break;
        case 3:
            tel_tatebayasi_main();
            break;
        case 4:
            tel_home_ring_dial_main();
            break;
        case 5:
            tel_tome_main();
            break;
        case 6:
            tel_call_girl_main();
            break;
        case 7:
            tel_call_girl_dialog2_main();
            break;
        case 8:
            tel_basyo_main();
            break;
        case 9:
            tel_sub_sub_dispatch();
            break;
    }
    hizuke_show();
    message_window_show();
    parameter_show();
    icon_cursor_move(0);
    step_directional_scroll_once();
    basyo_show();
    dec_bg_show(0);
    update_anim_state(0);
    if (mascot_cw_disp & 0x80) {
        mascot_move();
    }
}
