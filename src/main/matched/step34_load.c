/* Debug Symbol: step34_load (from o.bin, exact match) */
/* Address: 0x8005ADDC */
/* Size: 132 bytes (33 instructions) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 get_season(void);
extern s32 value_cache_find_or_load(s32 ptr, s32 arg1);
extern void indexed_byte_lsb_set(s32 arg0, s32 arg1);
extern void set_dec_bri(s32 arg0);
extern void sub_step_up(void);
extern void sub_sub_step_up(void);

extern s32 step34_load_tbl0[];
extern u8 screen_bri;

void step34_load(void) {
    s32 idx;
    s32 result;

    idx = get_season();
    result = value_cache_find_or_load(step34_load_tbl0[idx], 0);

    if (result != -1) {
        if (result == 0 || result == 1) {
            indexed_byte_lsb_set(0, result);
            set_dec_bri(0x80);
            screen_bri = 0x80;
            sub_step_up();
        }
    } else {
        sub_sub_step_up();
    }
}
