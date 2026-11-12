/* @RodataBase: 0x8014C540 */
/* @SymbolStart: 0x8013B1F0 */
/* @SymbolSize: 1948 */
/* event_bustup_cd_read -- ETC overlay, Address: 0x8013B1F8, Size: 1940 bytes (485 insns). */
#include "types.h"

extern s32 etc_bustup_flag;

extern void bustup_cd_read(s32);
extern void etc_bustup_year_read(s32);
extern void etc_bustup_id32_girl_read(s32);
extern void etc_bustup_id34_girl_read(s32);
extern void etc_bustup_kind19_girl_read(s32);
extern void etc_bustup_pair_read(s32, s32);
extern void etc_bustup_kind14_id11_girl_read(s32);
extern void etc_bustup_kind14_id12_girl_read(s32);
extern void etc_bustup_kind14_id13_girl_read(s32);
extern void etc_bustup_kind10_id48_girl_read(s32);
extern void etc_bustup_kind5_low_girl_read(s32);
extern void etc_bustup_kind5_high_girl_read(s32);
extern void etc_bustup_kind4_girl_read(s32);
extern void etc_bustup_month_read(s32, s32);
extern void etc_bustup_season_read(s32, s32);

extern s32 etc_bustup_flag;

void event_bustup_cd_read(s32 a0, s32 a1, s32 a2, s32 a3)
{
    etc_bustup_flag = 0;
    switch ((u32) a0) {
    case 0:
        switch ((u32) a1) {
        case 29:
        case 30:
        case 40:
            etc_bustup_year_read(a2);
            return;
        case 32:
            etc_bustup_id32_girl_read(a2);
            return;
        case 34:
            etc_bustup_id34_girl_read(a2);
            return;
        default:
            etc_bustup_season_read(a2, a3);
            return;
        }
    case 1:
        switch ((u32) a1) {
        case 32:
            etc_bustup_id32_girl_read(a2);
            return;
        case 34:
            etc_bustup_id34_girl_read(a2);
            return;
        default:
            etc_bustup_season_read(a2, a3);
            return;
        }
    case 2:
        switch ((u32) a1) {
        case 20:
            etc_bustup_season_read(a2, a3);
            return;
        case 1:
        case 34:
        case 53:
        case 61:
        case 68:
        case 74:
        case 79:
        case 81:
            etc_bustup_month_read(a2, a3);
            return;
        case 76:
            etc_bustup_season_read(a2, a3);
            return;
        case 78:
            etc_bustup_year_read(a2);
            return;
        default:
            return;
        }
    case 3:
        etc_bustup_month_read(a2, a3);
        return;
    case 4:
        if (a1 == 1 && a2 == 9) {
            etc_bustup_flag = 1;
            bustup_cd_read(26168);
            return;
        }
        etc_bustup_kind4_girl_read(a2);
        return;
    case 5:
        if (a1 < 3) {
            etc_bustup_kind5_low_girl_read(a2);
            return;
        }
        etc_bustup_kind5_high_girl_read(a2);
        return;
    case 6:
        etc_bustup_month_read(a2, a3);
        return;
    case 7:
        etc_bustup_month_read(a2, a3);
        return;
    case 8:
        switch ((u32) a1) {
        case 22:
        case 23:
        case 24:
            etc_bustup_pair_read(2, 2);
            return;
        case 31:
            etc_bustup_pair_read(3, 4);
            return;
        case 41:
        case 42:
            etc_bustup_pair_read(4, 7);
            return;
        case 50:
        case 51:
            etc_bustup_pair_read(5, 8);
            return;
        case 60:
            etc_bustup_flag = 1;
            bustup_cd_read(25673);
            return;
        case 61:
            etc_bustup_pair_read(6, 9);
            return;
        case 90:
            etc_bustup_flag = 1;
            bustup_cd_read(26168);
            return;
        case 14:
        case 70:
            return;
        default:
            etc_bustup_month_read(a2, a3);
            return;
        }
    case 9:
        etc_bustup_month_read(a2, a3);
        return;
    case 10:
        switch ((u32) a1) {
        case 32:
            etc_bustup_id32_girl_read(a2);
            return;
        case 34:
            etc_bustup_id34_girl_read(a2);
            return;
        case 37:
        case 41:
        case 42:
        case 43:
        case 44:
            etc_bustup_month_read(a2, a3);
            return;
        case 46:
            etc_bustup_kind5_low_girl_read(a2);
            return;
        case 50:
            etc_bustup_kind5_high_girl_read(a2);
            return;
        case 48:
        case 49:
            etc_bustup_kind10_id48_girl_read(a2);
            return;
        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
        case 56:
        case 57:
        case 58:
        case 59:
        case 60:
        case 61:
            etc_bustup_pair_read(a2, a1 - 51);
            return;
        default:
            etc_bustup_season_read(a2, a3);
            return;
        }
    case 11:
        if (a1 == 30 || a1 == 29 || a1 == 40) {
            etc_bustup_flag = 1;
            bustup_cd_read(27608);
            return;
        }
        etc_bustup_season_read(a2, a3);
        return;
    case 12:
        etc_bustup_season_read(a2, a3);
        return;
    case 13:
        switch ((u32) a1) {
        case 32:
            etc_bustup_id32_girl_read(a2);
            return;
        case 34:
            etc_bustup_id34_girl_read(a2);
            return;
        case 38:
        case 39:
        case 40:
        case 41:
        case 43:
            etc_bustup_month_read(a2, a3);
            return;
        case 42:
            if (a2 == 7 || a2 == 8 || a2 == 9) {
                etc_bustup_season_read(a2, a3);
                return;
            }
            etc_bustup_month_read(a2, a3);
            return;
        case 44:
            etc_bustup_kind5_low_girl_read(a2);
            return;
        case 45:
            etc_bustup_kind5_high_girl_read(a2);
            return;
        case 46:
        case 47:
        case 48:
        case 49:
        case 50:
        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
        case 56:
            etc_bustup_pair_read(a2, a1 - 46);
            return;
        default:
            etc_bustup_season_read(a2, a3);
            return;
        }
    case 14:
        if (a1 == 11) {
            etc_bustup_kind14_id11_girl_read(a2);
            return;
        }
        if (a1 == 12) {
            etc_bustup_kind14_id12_girl_read(a2);
            return;
        }
        if (a1 == 13) {
            etc_bustup_kind14_id13_girl_read(a2);
            return;
        }
        etc_bustup_pair_read(a2, a1);
        return;
    case 15:
        etc_bustup_month_read(a2, a3);
        return;
    case 16:
        etc_bustup_pair_read(a2, a1);
        return;
    case 17:
        return;
    case 18:
        switch ((u32) a1) {
        case 5:
            etc_bustup_pair_read(a2, 6);
            return;
        case 6:
            etc_bustup_pair_read(a2, 7);
            return;
        case 7:
            etc_bustup_pair_read(a2, 8);
            return;
        case 8:
            etc_bustup_pair_read(a2, 9);
            return;
        case 9:
            etc_bustup_pair_read(a2, 10);
            return;
        default:
            etc_bustup_month_read(a2, a3);
            return;
        }
    case 19:
        etc_bustup_kind19_girl_read(a2);
        return;
    case 20:
        switch ((u32) a1) {
        case 6:
        case 7:
        case 29:
            etc_bustup_pair_read(a2, 1);
            return;
        case 10:
        case 11:
        case 30:
            etc_bustup_pair_read(a2, 2);
            return;
        case 18:
        case 19:
        case 32:
        case 33:
            etc_bustup_pair_read(a2, 4);
            return;
        default:
            etc_bustup_month_read(a2, a3);
            return;
        }
    case 21:
        if (a2 == 12 || a2 == 11) {
            etc_bustup_month_read(a2, a3);
            return;
        }
        switch ((u32) a1) {
        case 0:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            etc_bustup_month_read(a2, a3);
            return;
        case 12:
            etc_bustup_flag = 1;
            bustup_cd_read(23918);
            return;
        default:
            etc_bustup_season_read(a2, a3);
            return;
        }
    default:
        return;
    }
}
