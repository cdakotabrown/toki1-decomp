/* @RodataBase: 0x8013CD4C */
/* @SymbolStart: 0x80138AAC */
/* @SymbolSize: 192 */
/* Overlay: OPTION */
#include "types.h"

extern void option_screen_init();
extern void option_screen_fade_in();
extern void option_screen_menu();
extern void option_list_main3();
extern void option_list_main4();
extern void option_list_main5();
extern void game_reset();

typedef struct {
    s32 counter;
    u8 pad[2];
    u8 sel;
} OptState;

extern OptState step_counter;

void option_screen_dispatch(void) {
    step_counter.counter = step_counter.counter + 1;
    switch (step_counter.sel) {
        case 0: option_screen_init(); break;
        case 1: option_screen_fade_in(); break;
        case 2: option_screen_menu(); break;
        case 3: option_list_main3(); break;
        case 4: option_list_main4(); break;
        case 5: option_list_main5(); break;
        default: game_reset(); break;
    }
}
