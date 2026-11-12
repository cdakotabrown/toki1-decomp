/* @SymbolStart: 0x80135A70 */
/* @SymbolSize: 204 */
/* @RodataBase: 0x80138E9C */
/* olh_trivia_page_main - OLH overlay, Address: 0x80135A78, Size: 196 bytes (49 insns) */
#include "types.h"

extern u8 now_sub_sub_step;

extern void olh_trivia_page_init(void);
extern void olh_main70_menu1(void);
extern void olh_trivia_soft_reset_help(void);
extern void olh_trivia_autofire_help(void);
extern void olh_trivia_sd_speed_help(void);
extern void olh_trivia_location_window_help(void);
extern void olh_trivia_lyrics_help(void);
extern void olh_trivia_kanji_dict_help(void);
extern void olh_trivia_sound_fail_help(void);

void olh_trivia_page_main(void) {
    u8 idx;

    idx = now_sub_sub_step;
    switch (idx) {
    case 0:
        olh_trivia_page_init();
        break;
    case 1:
        olh_main70_menu1();
        break;
    case 16:
        olh_trivia_soft_reset_help();
        break;
    case 32:
        olh_trivia_autofire_help();
        break;
    case 48:
        olh_trivia_sd_speed_help();
        break;
    case 64:
        olh_trivia_location_window_help();
        break;
    case 80:
        olh_trivia_lyrics_help();
        break;
    case 96:
        olh_trivia_kanji_dict_help();
        break;
    case 112:
        olh_trivia_sound_fail_help();
        break;
    }
}
