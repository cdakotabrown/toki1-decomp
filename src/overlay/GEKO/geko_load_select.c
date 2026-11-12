/* @SymbolStart: 0x80138F34 */
/* @SymbolSize: 428 */
/* @RodataBase: 0x80144878 */
/* Address: 0x80138F3C, Size: 420 bytes (105 instructions) */
#include "types.h"

extern void cd_read_start2();
extern void game_reset();
extern void geko_ptr_tbl_init();
extern void sub_sub_step_up();
extern u8 now_girl;

void geko_load_select(void) {
    s32 selector = now_girl;

    switch (selector) {
        case 0:
            cd_read_start2(6, 0x80198000, 0xAE25);
            break;
        case 1:
            cd_read_start2(3, 0x80198000, 0xAE39);
            break;
        case 2:
            cd_read_start2(4, 0x80198000, 0xAE2B);
            break;
        case 3:
            cd_read_start2(5, 0x80198000, 0xAE2F);
            break;
        case 4:
            cd_read_start2(7, 0x80198000, 0xAE4C);
            break;
        case 5:
            cd_read_start2(5, 0x80198000, 0xAE3C);
            break;
        case 6:
            cd_read_start2(6, 0x80198000, 0xAE41);
            break;
        case 7:
            cd_read_start2(5, 0x80198000, 0xAE34);
            break;
        case 8:
            cd_read_start2(4, 0x80198000, 0xAE21);
            break;
        case 9:
            cd_read_start2(5, 0x80198000, 0xAE47);
            break;
        case 10:
            cd_read_start2(6, 0x80198000, 0xAE5A);
            break;
        case 11:
        case 12:
        case 13:
            cd_read_start2(7, 0x80198000, 0xAE53);
            break;
        default:
            game_reset();
            break;
    }

    geko_ptr_tbl_init();
    sub_sub_step_up();
}
