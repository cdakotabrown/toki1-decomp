/* @SymbolStart: 0x8013D48C */
/* @RodataBase: 0x801439C4 */
/* Address: 0x8013D494  (window starts at the absorbed cross-prologue head */
/* Size: 428 bytes (107 insns) */
/* @SymbolSize: 428 */
#include "types.h"

extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void shougatu_ptrs3_set(void);
extern void sub_sub_step_up(void);
extern u8 now_girl;

void shougatu_sprite_load(void) {
    switch (now_girl) {
    case 0:  cd_read_start2(6, 0x80198000, 0xAE25); break;
    case 1:  cd_read_start2(3, 0x80198000, 0xAE39); break;
    case 2:  cd_read_start2(4, 0x80198000, 0xAE2B); break;
    case 3:  cd_read_start2(5, 0x80198000, 0xAE2F); break;
    case 4:  cd_read_start2(7, 0x80198000, 0xAE4C); break;
    case 5:  cd_read_start2(5, 0x80198000, 0xAE3C); break;
    case 6:  cd_read_start2(6, 0x80198000, 0xAE41); break;
    case 7:  cd_read_start2(5, 0x80198000, 0xAE34); break;
    case 8:  cd_read_start2(4, 0x80198000, 0xAE21); break;
    case 9:  cd_read_start2(5, 0x80198000, 0xAE47); break;
    case 10: cd_read_start2(6, 0x80198000, 0xAE5A); break;
    case 11:
    case 12:
    case 13:
        cd_read_start2(7, 0x80198000, 0xAE53);
        break;
    default:
        game_reset();
        break;
    }
    shougatu_ptrs3_set();
    sub_sub_step_up();
}
