/* Address: 0x80073AF4 */
/* Size: 152 bytes (38 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 byouki_kind;
extern u16 byouki_state;
extern s32 sched_icon;
extern s32 step34_load_tbl0[];
extern s32 get_season(void);
extern s32 value_cache_find_or_load(s32, s32);
extern s32 get_weekly_bg_sector(void);

s32 week_day_bg_load(void) {
    s32 ret;
    s32 idx;

    if (((byouki_kind & 0xF) == 3) && ((byouki_state & 1) == 0) && ((sched_icon & 0xF) == 7)) {
        idx = get_season();
        ret = value_cache_find_or_load(step34_load_tbl0[idx], 0);
    } else {
        ret = value_cache_find_or_load(get_weekly_bg_sector(), 0);
    }

    return ret;
}
