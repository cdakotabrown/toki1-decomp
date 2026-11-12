/* Function: vacation_day_bg_load */
/* Address: 0x80074DDC */
/* Size: 184 bytes */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 schedule_buf;
extern u8 byouki_kind;
extern s32 sched_icon;
extern s32 basyo_bg_load_tbl0[];

extern s32 get_holiday_bg_sector(void);
extern s32 value_cache_find_or_load(s32 arg0, s32 arg1);
extern s32 get_season(void);

s32 vacation_day_bg_load(void) {
    u8 mode;
    s32 ret;
    s32 idx;

    mode = schedule_buf;

    switch (mode) {
    case 37:
    case 66:
        ret = get_holiday_bg_sector();
        ret = value_cache_find_or_load(ret, 0);
        break;
    default:
        if (((byouki_kind & 0xF) == 3) && ((sched_icon & 0xF) == 7)) {
            idx = get_season();
            ret = value_cache_find_or_load(basyo_bg_load_tbl0[idx], 0);
        } else {
            ret = get_holiday_bg_sector();
            ret = value_cache_find_or_load(ret, 0);
        }
        break;
    }

    return ret;
}
