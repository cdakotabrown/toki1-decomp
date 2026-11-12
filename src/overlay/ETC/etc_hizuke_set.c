/* @SymbolSize: 496 */
/* @RodataBase: 0x8014CA90 */
#include "types.h"

extern u8 option_on;
extern volatile u8 screen_w[];
extern volatile u8 album_work[];
extern s32 etc_record_index;

extern volatile s32 etc_rec_kind;
extern volatile s32 etc_rec_id;
extern volatile s32 etc_rec_variant;
extern volatile s32 etc_rec_girl;
extern volatile s32 etc_rec_season;
extern volatile s32 etc_rec_year;
extern volatile s32 etc_rec_month;
extern volatile s32 etc_rec_day;
extern volatile s32 etc_rec_youbi;

void etc_hizuke_set(void)
{
    volatile u8 *p;

    if (option_on == 0) {
        p = screen_w + 8 * etc_record_index;
        etc_rec_girl = p[1886];
        etc_rec_id = p[1887];
        etc_rec_kind = p[1888];
        etc_rec_variant = (u32)p[1889] % 6U;
        etc_rec_year = p[1890];
        etc_rec_month = p[1891];
        etc_rec_day = p[1892];
        etc_rec_youbi = p[1893];
        switch (p[1891]) {
        case 3:
        case 4:
        case 5:
            etc_rec_season = 0;
            break;
        case 6:
        case 7:
        case 8:
            etc_rec_season = 1;
            break;
        case 9:
        case 10:
        case 11:
            etc_rec_season = 2;
            break;
        case 1:
        case 2:
        case 12:
            etc_rec_season = 3;
            break;
        }
        return;
    }
        p = album_work + 8 * etc_record_index;
        etc_rec_girl = p[88];
        etc_rec_id = p[89];
        etc_rec_kind = p[90];
        etc_rec_variant = (u32)p[91] % 6U;
        etc_rec_year = p[92];
        etc_rec_month = p[93];
        etc_rec_day = p[94];
        etc_rec_youbi = p[95];
        switch (p[93]) {
        case 3:
        case 4:
        case 5:
            etc_rec_season = 0;
            break;
        case 6:
        case 7:
        case 8:
            etc_rec_season = 1;
            break;
        case 9:
        case 10:
        case 11:
            etc_rec_season = 2;
            break;
        case 1:
        case 2:
        case 12:
            etc_rec_season = 3;
            break;
        }
}
