/* @SymbolStart: 0x8013E354 */
/* @SymbolSize: 448 */
/* Address: 0x8013E35C (window starts 0x8013E354 - absorbed cross-prologue head) */
/* Size: 448 bytes (112 instructions, head included) */
#include "types.h"

extern void etc_hizuke_bar_disp(s32, s32);
extern void etc_number_disp(s32, s32, s32, s32);

extern s32 etc_rec_year;
extern s32 etc_rec_month;
extern s32 etc_rec_day;

void etc_hizuke_show(void)
{
    if (etc_rec_day < 10 && etc_rec_month < 10) {
        etc_number_disp(etc_rec_year, 80, 45, 0);
        etc_number_disp(-etc_rec_month, 100, 45, 0);
        etc_hizuke_bar_disp(229, 62);
    } else if (etc_rec_day >= 10 && etc_rec_month < 10) {
        etc_number_disp(etc_rec_year, 75, 45, 0);
        etc_number_disp(-etc_rec_month, 95, 45, 0);
        etc_hizuke_bar_disp(224, 66);
    } else if (etc_rec_day < 10 && etc_rec_month >= 10) {
        etc_number_disp(etc_rec_year, 75, 45, 0);
        etc_number_disp(-etc_rec_month, 100, 45, 0);
        etc_hizuke_bar_disp(224, 66);
    } else if (etc_rec_day >= 10 && etc_rec_month >= 10) {
        etc_number_disp(etc_rec_year, 70, 45, 0);
        etc_number_disp(-etc_rec_month, 95, 45, 0);
        etc_hizuke_bar_disp(219, 71);
    }

    etc_number_disp(-etc_rec_day, 120, 45, 0);
}
