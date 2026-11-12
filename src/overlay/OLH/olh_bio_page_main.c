/* @SymbolStart: 0x80133B20 */
/* @SymbolSize: 204 */
/* @RodataBase: 0x80137F48 */
/* olh_bio_page_main - OLH overlay, Address: 0x80133B28, Size: 196 bytes (49 insns) */
#include "types.h"

extern u8 now_sub_sub_step;

extern void olh_page40_init(void);
extern void olh_main40_menu1(void);
extern void olh_bio_graph_help(void);
extern void olh_bio_blue_line_help(void);
extern void olh_bio_distance_help(void);
extern void olh_bio_red_help(void);
extern void olh_bio_yellow_help(void);
extern void olh_bio_green_help(void);
extern void olh_bio_clear_need_help(void);

void olh_bio_page_main(void) {
    u8 idx;

    idx = now_sub_sub_step;
    switch (idx) {
    case 0:
        olh_page40_init();
        break;
    case 1:
        olh_main40_menu1();
        break;
    case 16:
        olh_bio_graph_help();
        break;
    case 32:
        olh_bio_blue_line_help();
        break;
    case 48:
        olh_bio_distance_help();
        break;
    case 64:
        olh_bio_red_help();
        break;
    case 80:
        olh_bio_yellow_help();
        break;
    case 96:
        olh_bio_green_help();
        break;
    case 112:
        olh_bio_clear_need_help();
        break;
    }
}
