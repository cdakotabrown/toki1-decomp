/* Function: holiday */
/* Address: 0x800753E0  (registered address corrected 2026-09-17; see below) */
/* Size: 328 bytes (82 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @SymbolStart: 0x800753E0 */
/* @SymbolSize: 328 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_registers_liveness --target-exe disks/SLPS_000.65 --address 0x800753E0 */
#include "types.h"

extern u8 now_sub_step;
extern u8 mascot_cw_disp;

extern void holiday_init(void);
extern void holiday_disp(void);
extern void holiday_main5(void);
extern void holiday_club(void);
extern void dispatch_sub_sub_step_mode(void);
extern void holiday_tel(void);
extern void magazine_step(void);
extern void cal_menu(void);
extern void system_config_main(void);
extern void machiawase_main(void);
extern void mascot_move(void);
extern void message_window_show(void);
extern void dec_bg_show(s32);
extern void hizuke_show(void);
extern void parameter_show(void);
extern void step_directional_scroll_once(void);
extern void update_anim_state(s32);

void holiday(void) {
    u32 sw = now_sub_step;

    if (sw >= 65u) {
        if (sw != 254u) {
            goto epilogue;
        }
        goto case254;
    }
    switch (sw) {

        case 0: holiday_init(); break;
        case 1: holiday_disp(); break;
        case 5: holiday_main5(); break;
        case 6: holiday_club(); break;
        case 7: dispatch_sub_sub_step_mode(); break;
        case 16: holiday_tel(); break;
        case 32: magazine_step(); break;
        case 48: cal_menu(); break;
        case 64: system_config_main(); break;
        default:
            break;
    }
    goto epilogue;
case254:
    machiawase_main();
epilogue:
    if (mascot_cw_disp & 0x80) {
        mascot_move();
    }
    message_window_show();
    dec_bg_show(0);
    hizuke_show();
    parameter_show();
    step_directional_scroll_once();
    update_anim_state(1);
}
