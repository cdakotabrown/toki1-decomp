/* @SymbolStart: 0x80132F50 */
/* @SymbolSize: 204 */
/* @RodataBase: 0x80137AE0 */
/* olh_card_page_main - OLH overlay, Address: 0x80132F58, Size: 196 bytes (49 insns) */
#include "types.h"

extern u8 now_sub_sub_step;

extern void olh_page20_init(void);
extern void olh_card_menu_select(void);
extern void olh_card_album_block_help(void);
extern void olh_card_sysfile_help(void);
extern void olh_card_no_remove_help(void);
extern void olh_card_full_help(void);
extern void olh_memcard_menu_step1(void);
extern void olh_memcard_menu_step2(void);
extern void olh_memcard_menu_step3(void);

void olh_card_page_main(void) {
    u8 idx;

    idx = now_sub_sub_step;
    switch (idx) {
    case 0:
        olh_page20_init();
        break;
    case 1:
        olh_card_menu_select();
        break;
    case 16:
        olh_card_album_block_help();
        break;
    case 32:
        olh_card_sysfile_help();
        break;
    case 48:
        olh_card_no_remove_help();
        break;
    case 64:
        olh_card_full_help();
        break;
    case 80:
        olh_memcard_menu_step1();
        break;
    case 96:
        olh_memcard_menu_step2();
        break;
    case 112:
        olh_memcard_menu_step3();
        break;
    }
}
