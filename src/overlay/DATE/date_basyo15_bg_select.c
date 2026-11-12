/* @SymbolStart: 0x80138544 */
/* @SymbolSize: 260 */
/* @RodataBase: 0x8015AF18 */
#include "types.h"

extern u8 date_zoo_season;

extern void dec_bg_cache_step(s32 a0);
extern void sub_sub_step_up(void);

void date_basyo15_bg_select(void) {
    switch (date_zoo_season) {
    case 0:
        dec_bg_cache_step(17604);
        break;
    case 1:
        dec_bg_cache_step(17528);
        break;
    case 2:
        dec_bg_cache_step(17566);
        break;
    case 3:
        dec_bg_cache_step(17584);
        break;
    case 4:
        dec_bg_cache_step(17538);
        break;
    case 5:
        dec_bg_cache_step(17556);
        break;
    case 6:
        dec_bg_cache_step(17604);
        break;
    case 7:
        dec_bg_cache_step(17575);
        break;
    case 8:
        dec_bg_cache_step(17547);
        break;
    case 9:
        dec_bg_cache_step(17604);
        break;
    case 10:
        dec_bg_cache_step(17594);
        break;
    case 11:
        dec_bg_cache_step(17613);
        break;
    }
    sub_sub_step_up();
}
