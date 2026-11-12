/* Likely Symbol: none -- a stale 2026-04 guess read menu_step_mainff here; menu_step_mainff is 0x8005BE0C */
/* Address: 0x8005B62C */
/* Size: 160 bytes (40 instructions) */
/* Region: GCC (0x80041000-0x80080FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void k_disp_inc(void);
extern void parameter_show(void);
extern void hizuke_show(void);
extern void message_window_show(void);
extern void icon_cursor_move(s32);
extern void step_directional_scroll_once(void);
extern void basyo_show(void);
extern void dec_bg_show(s32);
extern s32 icon_select_check(void);
extern s32 voice_click_check(void);
extern s32 hotspot_table_check(void);
extern s32 hotspot_check(void);
extern void hotspot_step_check(void);

void step34_disp(void) {
    k_disp_inc();
    parameter_show();
    hizuke_show();
    message_window_show();
    icon_cursor_move(0);
    step_directional_scroll_once();
    basyo_show();
    dec_bg_show(0);

    if (icon_select_check() != 0) {
        return;
    }
    if (voice_click_check() != 0) {
        return;
    }
    if (hotspot_table_check() != 0) {
        return;
    }
    if (hotspot_check() != 0) {
        return;
    }
    hotspot_step_check();
}
