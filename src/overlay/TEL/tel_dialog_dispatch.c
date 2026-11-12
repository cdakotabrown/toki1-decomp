/* @SymbolStart: 0x80134B0C */
/* @RodataBase: 0x8013F3D8 */
/* Address: 0x80134B28 */
/* Size: 420 bytes (105 insns) */
/* @SymbolSize: 448 */
#include "types.h"

typedef struct {
    s32 count;
    u8  pad[2];
    u8  sel;
} StepState;

extern StepState step_counter;

extern void tel_ring0(void);
extern void tel_greet1(void);
extern void tel_dialog2(void);
extern void tel_call_start_step(void);
extern void tel_dialog4(void);
extern void tel_dialog8(void);
extern void tel_dialog9(void);
extern void tel_dialoga(void);
extern void tel_dialog5(void);
extern void tel_dialog7(void);
extern void tel_dialog6(void);
extern void tel_rusu10(void);
extern void tel_dialog20(void);
extern void tel_cal_select30(void);
extern void tel_dialog40(void);
extern void tel_dialog41(void);
extern void tel_dialog42(void);
extern void tel_dialog43(void);
extern void tel_dialog44(void);
extern void tel_dialog45(void);
extern void update_anim_state(s32);
extern void parameter_show(void);
extern void message_window_show(void);
extern void hizuke_show(void);
extern void mascot_move(void);
extern void step_directional_scroll_once(void);

void tel_dialog_dispatch(void) {
    step_counter.count++;
    switch (step_counter.sel) {
    case 0:  tel_ring0(); break;
    case 1:  tel_greet1(); break;
    case 2:  tel_dialog2(); break;
    case 3:  tel_call_start_step(); break;
    case 4:  tel_dialog4(); break;
    case 8:  tel_dialog8(); break;
    case 9:  tel_dialog9(); break;
    case 10: tel_dialoga(); break;
    case 5:  tel_dialog5(); break;
    case 7:  tel_dialog7(); break;
    case 6:  tel_dialog6(); break;
    case 16: tel_rusu10(); break;
    case 48: tel_dialog20(); break;
    case 32: tel_cal_select30(); break;
    case 64: tel_dialog40(); break;
    case 65: tel_dialog41(); break;
    case 66: tel_dialog42(); break;
    case 67: tel_dialog43(); break;
    case 68: tel_dialog44(); break;
    case 69: tel_dialog45(); break;
    }
    update_anim_state(1);
    parameter_show();
    message_window_show();
    hizuke_show();
    mascot_move();
    step_directional_scroll_once();
}
