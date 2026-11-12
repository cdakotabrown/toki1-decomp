/* @SymbolStart: 0x80138290 */
/* @SymbolSize: 244 */
/* @RodataBase: 0x8015AEEC */
#include "types.h"

extern u8 date_parade_season;

extern void dec_bg_cache_step(s32 a0);
extern void sub_sub_step_up(void);

void date_basyo14_bg_select(void) {
    switch (date_parade_season) {
    case 0:
        dec_bg_cache_step(17410);
        break;
    case 1:
        dec_bg_cache_step(17419);
        break;
    case 2:
        dec_bg_cache_step(17428);
        break;
    case 3:
        dec_bg_cache_step(17436);
        break;
    case 4:
        dec_bg_cache_step(17446);
        break;
    case 5:
        dec_bg_cache_step(17455);
        break;
    case 6:
        dec_bg_cache_step(17491);
        break;
    case 7:
        dec_bg_cache_step(17472);
        break;
    case 8:
        dec_bg_cache_step(17481);
        break;
    case 9:
        dec_bg_cache_step(17501);
        break;
    case 10:
        dec_bg_cache_step(17463);
        break;
    }
    sub_sub_step_up();
}
