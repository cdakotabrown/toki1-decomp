/* @SymbolStart: 0x801364C0 */
/* @SymbolSize: 204 */
/* @RodataBase: 0x80139478 */
/* olh_file_page_main - OLH overlay, Address: 0x801364C8, Size: 196 bytes (49 insns) */
#include "types.h"

extern u8 now_sub_sub_step;

extern void olh_file_page_init(void);
extern void olh_main30_menu1(void);
extern void olh_file_system_help(void);
extern void olh_file_copy_help(void);
extern void olh_file_restart_help(void);
extern void olh_file_delete_help(void);
extern void olh_file_clear_count_help(void);
extern void olh_file_selected_album_help(void);
extern void olh_file_param_view_help(void);

void olh_file_page_main(void) {
    u8 idx;

    idx = now_sub_sub_step;
    switch (idx) {
    case 0:
        olh_file_page_init();
        break;
    case 1:
        olh_main30_menu1();
        break;
    case 16:
        olh_file_system_help();
        break;
    case 32:
        olh_file_copy_help();
        break;
    case 48:
        olh_file_restart_help();
        break;
    case 64:
        olh_file_delete_help();
        break;
    case 80:
        olh_file_clear_count_help();
        break;
    case 96:
        olh_file_selected_album_help();
        break;
    case 112:
        olh_file_param_view_help();
        break;
    }
}
