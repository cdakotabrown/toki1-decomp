/* Address: 0x80060CBC */
/* Size: 160 bytes */
/* Compiler: GCC (region 0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 get_CD_State2(void);
extern s32 dec_bg_init(s32 arg);
extern void indexed_byte_lsb_set(s32 arg0, s32 arg1);
extern void dec_bg_reset(void);
extern s32 get_season(void);
extern void value_cache_find_or_load(s32 arg0, s32 arg1);
extern void set_dec_bri(s32 arg);
extern void goto_sub_step(s32 arg);

extern s32 dec_bg_handle;
extern s32 step34_load_tbl0[];
extern u8 screen_bri;

void uwasa_init1(void) {
    s32 result;
    s32 idx;
    s32 call_result;

    if (get_CD_State2() & 1) {
        call_result = dec_bg_init(0);
        indexed_byte_lsb_set(0, call_result);

        if (dec_bg_handle == -1) {
            dec_bg_reset();
            idx = get_season();
            value_cache_find_or_load(step34_load_tbl0[idx], 0);
        } else {
            set_dec_bri(128);
            screen_bri = 128;
            goto_sub_step(1);
        }
    }
}
