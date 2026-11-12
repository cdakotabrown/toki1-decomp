/* Function: step34_main0 */
/* Address: 0x8005A508 */
/* Size: 196 bytes (49 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @SymbolStart: 0x8005A4F8 */
/* @SymbolSize: 212 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v1:8 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u32 step_counter;
extern u8 now_sub_sub_step;
extern void step34_init0(void);
extern void step34_init1(void);
extern void step34_init2(void);
extern void step34_init3(void);
extern void hizuke_show(void);
extern void message_window_show(void);
extern void parameter_show(void);
extern void icon_cursor_move(s32);
extern void step_directional_scroll_once(void);
extern void basyo_show(void);
extern s32 get_last_gamen_mode(void);
extern void dec_bg_show(s32);

void step34_main0(void) {
    step_counter++;
    {
        s32 mode = now_sub_sub_step;
        switch (mode) {
        case 0:
            step34_init0();
            break;
        case 1:
            step34_init1();
            break;
        case 2:
            step34_init2();
            break;
        case 3:
            step34_init3();
            break;
        }
    }
    hizuke_show();
    message_window_show();
    parameter_show();
    icon_cursor_move(0);
    step_directional_scroll_once();
    basyo_show();
    dec_bg_show(get_last_gamen_mode());
}
