/* @SymbolStart: 0x80134EF4 */
/* @SymbolSize: 204 */
/* @RodataBase: 0x80138940 */
/* olh_minigame_page_main - OLH overlay, Address: 0x80134EFC, Size: 196 bytes (49 insns) */
#include "types.h"

extern u8 now_sub_sub_step;

extern void olh_minigame_page_init(void);
extern void olh_main60_menu1(void);
extern void olh_minigame_speed_help(void);
extern void olh_minigame_roll_item_help(void);
extern void olh_minigame_score_save_help(void);
extern void olh_minigame_card_help(void);
extern void olh_minigame_menu_step1(void);
extern void olh_minigame_menu_step2(void);
extern void olh_minigame_menu_step3(void);

void olh_minigame_page_main(void) {
    u8 idx;

    idx = now_sub_sub_step;
    switch (idx) {
    case 0:
        olh_minigame_page_init();
        break;
    case 1:
        olh_main60_menu1();
        break;
    case 16:
        olh_minigame_speed_help();
        break;
    case 32:
        olh_minigame_roll_item_help();
        break;
    case 48:
        olh_minigame_score_save_help();
        break;
    case 64:
        olh_minigame_card_help();
        break;
    case 80:
        olh_minigame_menu_step1();
        break;
    case 96:
        olh_minigame_menu_step2();
        break;
    case 112:
        olh_minigame_menu_step3();
        break;
    }
}
