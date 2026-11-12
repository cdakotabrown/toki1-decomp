/* @SymbolStart: 0x80132060 */
/* @RodataBase: 0x8013EF68 */
/* @SymbolSize: 216 */
/* tel_yoshio_main - TEL overlay, Address: 0x8013207C, Size: 196 bytes (49 insns) */
#include "types.h"

typedef struct {
    s32 count;
    u8  unk4;
    u8  step;
    u8  sel;
} StepState;

extern StepState step_counter;

extern void tel_yoshio_ring(void);
extern void tel_yoshio_intro(void);
extern void tel_yoshio_menu(void);
extern void tel_dialog_disp(void);
extern void tel_yoshio_slot(void);
extern void update_anim_state(s32);
extern void parameter_show(void);
extern void message_window_show(void);
extern void hizuke_show(void);
extern void mascot_move(void);
extern void basyo_show(void);
extern void step_directional_scroll_once(void);

void tel_yoshio_main(void) {
    step_counter.count++;
    switch (step_counter.sel) {
        case 0:
            tel_yoshio_ring();
            break;
        case 1:
            tel_yoshio_intro();
            break;
        case 2:
            tel_yoshio_menu();
            break;
        case 3:
            tel_dialog_disp();
            break;
        case 4:
            tel_yoshio_slot();
            break;
    }
    update_anim_state(1);
    parameter_show();
    message_window_show();
    hizuke_show();
    mascot_move();
    basyo_show();
    step_directional_scroll_once();
}
