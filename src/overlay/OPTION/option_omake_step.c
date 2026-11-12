/* @SymbolStart: 0x80138860 */
/* @RodataBase: 0x8013CC08 */
/* @SymbolSize: 352 */
/* Overlay: OPTION */
/* Address: 0x80138868 */
/* Size: 360 bytes (90 instructions) */
/* @PostProc: pad_trailing_nop --count 4  (LEGITIMATE: body is byte-exact at */
#include "types.h"

extern void option_menu_in();
extern void option_menu();
extern void option_zoom2();
extern void option_subgame_init();
extern void option_subgame_main();
extern void option_subgame_exit();
extern void option_talk_init();
extern void option_talk_main();
extern void option_talk_xa_main();
extern void option_talk_exit();
extern void option_ending_init();
extern void option_select();
extern void option_select_exit();
extern void option_ending_load();
extern void option_load_wait();
extern void option_ending_start();
extern void option_exit10();
extern u8 now_sub_sub_step;

void option_omake_step(void)
{
    u32 hidden_t6;

    hidden_t6 = now_sub_sub_step;
    switch ((s32)hidden_t6) {
        case 0:
            option_menu_in();
            break;
        case 1:
            option_menu();
            break;
        case 2:
            option_zoom2();
            break;
        case 10:
            option_subgame_init();
            break;
        case 11:
            option_subgame_main();
            break;
        case 12:
            option_subgame_exit();
            break;
        case 20:
            option_talk_init();
            break;
        case 21:
            option_talk_main();
            break;
        case 22:
            option_talk_xa_main();
            break;
        case 23:
            option_talk_exit();
            break;
        case 30:
            option_ending_init();
            break;
        case 31:
            option_select();
            break;
        case 32:
            option_select_exit();
            break;
        case 33:
            option_ending_load();
            break;
        case 34:
            option_load_wait();
            break;
        case 35:
            option_ending_start();
            break;
        case 40:
            option_exit10();
            break;
        case 17: case 18: case 19:
        case 24: case 25: case 26: case 27: case 28: case 29:
        case 36: case 37: case 38: case 39:
            break;
    }
}
