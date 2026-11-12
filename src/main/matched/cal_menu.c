/* Address: 0x8005BA20, Size: 152 bytes */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v1:7 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern u8 now_sub_sub_step;
extern void cal_menu_init(void);
extern void cal_menu_main(void);
extern void cal_menu_exit(void);
extern void cal_bio_disp(void);
extern void parameter_show(void);
extern void hizuke_show(void);
extern void message_window_show(void);
extern void basyo_show(void);
extern void step_directional_scroll_once(void);
void cal_menu(void) {
    u8 m = now_sub_sub_step;
    switch (m) {
    case 0:
        cal_menu_init();
        break;
    case 1:
        cal_menu_main();
        break;
    case 2:
        cal_menu_exit();
        break;
    }
    cal_bio_disp();
    parameter_show();
    hizuke_show();
    message_window_show();
    basyo_show();
    step_directional_scroll_once();
}
