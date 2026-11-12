/* Address: 0x800773BC */
/* Size: 108 bytes (27 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x800773BC */
/* @SymbolSize: 108 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 byouki_kind;
extern s32 get_season(void);
extern s32 value_cache_find_or_load(s32, s32);
extern s32 get_holiday_bg_sector(void);
extern s32 basyo_bg_load_tbl0[];

s32 basyo_bg_load(void) {
    s32 ret;
    s32 result;
    if ((byouki_kind & 0xF) == 3) {
        result = get_season();
        ret = value_cache_find_or_load(basyo_bg_load_tbl0[result], 0);
    } else {
        result = get_holiday_bg_sector();
        ret = value_cache_find_or_load(result, 0);
    }
    return ret;
}
