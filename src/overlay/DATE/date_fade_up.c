/* @SymbolStart: 0x8013E984 */
/* @RodataBase: 0x8015AFFC */
/* @CompileFlags: -cckr */
#include "types.h"

extern u8  date_pattern;
extern u8  now_girl;
extern u8  now_sub_sub_step;
extern s32 serifu_no;

void fade_up_main();

date_fade_up()
{
    s32 unused;
    s32 old;

    old = now_sub_sub_step;
    if (date_pattern == 1) {
        switch (now_girl) {
        case 0:
        case 3:
        case 4:
        case 9:
            fade_up_main();
            if (old != now_sub_sub_step) {
                now_sub_sub_step--;
                date_score_apply();
            }
            return 0;
        case 5:
            if (serifu_no != 0) {
                fade_up_main();
                if (old != now_sub_sub_step) {
                    now_sub_sub_step--;
                    date_score_apply();
                }
                return 0;
            }
            break;
        case 8:
            if (serifu_no < 2) {
                fade_up_main();
                if (old != now_sub_sub_step) {
                    now_sub_sub_step--;
                    date_score_apply();
                }
                return 0;
            }
            break;
        }
    }
    return date_score_apply();
}
