/* Address: 0x8005BB98 */
/* Size: 464 bytes (116 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x8005BB90 */
/* @SymbolSize: 472 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 now_sub_sub_step;

extern void save_menu_init(void);
extern void save_card_main(void);
extern void menu_step_main2(void);
extern void save_file_select_init(void);
extern void save_file_select_main(void);
extern void save_file_write_main(void);
extern void menu_step_main6(void);
extern void menu_step_main7(void);
extern void data_load_confirm(void);
extern void continue_game_init(void);
extern void save_format_main(void);
extern void menu_step_mainb0(void);
extern void save_file_read_main(void);
extern void menu_step_maine0(void);
extern void menu_step_mainf0(void);
extern void menu_step_mainff(void);

extern void basyo_show(void);
extern void cal_base_show(void);
extern void message_window_show(void);
extern void parameter_show(void);
extern void hizuke_show(void);
extern void step_directional_scroll_once(void);

void menu_step_main(void) {
    u32 mode;

    mode = now_sub_sub_step;
    switch (mode) {
    case 0:
        save_menu_init();
        break;
    case 1:
        save_card_main();
        break;
    case 2:
        menu_step_main2();
        break;
    case 3:
        save_file_select_init();
        break;
    case 4:
        save_file_select_main();
        break;
    case 5:
        save_file_write_main();
        break;
    case 6:
        menu_step_main6();
        break;
    case 7:
        menu_step_main7();
        break;
    case 8:
        data_load_confirm();
        break;
    case 9:
        continue_game_init();
        break;
    case 160:
        save_format_main();
        break;
    case 176:
        menu_step_mainb0();
        break;
    case 192:
        save_file_read_main();
        break;
    case 224:
        menu_step_maine0();
        break;
    case 240:
        menu_step_mainf0();
        break;
    case 255:
        menu_step_mainff();
        break;
    }
    basyo_show();
    cal_base_show();
    message_window_show();
    parameter_show();
    hizuke_show();
    step_directional_scroll_once();
}
