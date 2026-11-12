/* Function: func_8005AE0C */
/* Address: 0x8005AE0C */
/* Size: 168 bytes (42 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolSize: 244 */
/* @PostProc: patch_stack_complete --old 24 --new 40 */
/* @PostProc: strip_leading_instructions --count 2 */
/* and `Size: 168` (42 instructions) cut the second one in half: */
/* 21 + 40 = 61 words = 244 bytes, so @SymbolSize: 244.  The three "absent" */
#include "types.h"

extern u8 screen_bri;
extern s32 dec_bg_handle;
extern s32 step34_load_tbl0[];

extern void indexed_byte_lsb_set(s32, s32);
extern void set_dec_bri(s32);
extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern s32 get_CD_State2(void);
extern s32 dec_bg_init(s32);
extern void dec_bg_reset(void);
extern s32 get_season(void);
extern void value_cache_find_or_load(s32, s32);
extern void goto_sub_step(s32);

void func_8005AE0C(s32 result) {
    s32 a1;

    a1 = result;
    if (result == 0 || result == 1) {
        indexed_byte_lsb_set(0, a1);
        set_dec_bri(128);
        screen_bri = 128;
        sub_step_up();
    } else {
        sub_sub_step_up();
    }
}

void step34_init3(void) {
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
        value_cache_find_or_load(step34_load_tbl0[idx], 0);
    } else {
        set_dec_bri(0x80);
        screen_bri = 0x80;
        goto_sub_step(1);
    }
}
