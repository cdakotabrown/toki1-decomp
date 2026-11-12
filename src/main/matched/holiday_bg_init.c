/* Address: 0x80075B18 */
/* Size: 148 bytes (37 instructions) */
/* Region: GCC (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 get_CD_State2(void);
extern s32 dec_bg_init(s32 arg0);
extern void indexed_byte_lsb_set(s32 arg0, s32 arg1);
extern void dec_bg_reset(void);
extern s32 get_season(void);
extern void value_cache_find_or_load(s32 arg0, s32 arg1);
extern void set_dec_bri(s32 arg0);
extern void sub_step_up(void);

extern s32 dec_bg_handle;
extern s32 basyo_bg_load_tbl0[];

void holiday_bg_init(void) {
    s32 result;
    s32 idx;

    result = get_CD_State2();
    if ((result & 1) == 0) {
        return;
    }

    result = dec_bg_init(0);
    indexed_byte_lsb_set(0, result);

    if (dec_bg_handle == -1) {
        dec_bg_reset();
        idx = get_season();
        value_cache_find_or_load(basyo_bg_load_tbl0[idx], 0);
    } else {
        set_dec_bri(0x80);
        sub_step_up();
    }
}
