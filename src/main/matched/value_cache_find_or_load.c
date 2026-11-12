/* Debug Symbol: value_cache_find_or_load (from o.bin, high match) */
/* Address: 0x80057794 */
/* Size: 240 bytes (60 instructions) */
/* Compiler: IDO 5.2 -O2 -G0 -mips1 */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "common.h"

extern void cd_read_start(s32 a0, s32 a1);

extern s32 date_effect_init_tbl0;
extern s32 date_effect_init_tbl1;
extern u8 bg_change_tbl1;
extern s32 date2_intro_script_tbl0;
extern s32 date2_intro_script_tbl1;
extern u8 bg_change_tbl0;

s32 value_cache_find_or_load(s32 arg0, s32 arg1) {
    s32 val;
    s32 idx;
    s32 neg_offset;

    val = arg0;

    if (arg1 == 1) {

        if (arg0 == date_effect_init_tbl0) {
            return 0;
        }
        if (val == date_effect_init_tbl1) {
            return 1;
        }
        cd_read_start(10, 0x80162000);
        idx = bg_change_tbl1;
        neg_offset = -(idx << 2);
        *(s32 *)((s32)&date_effect_init_tbl1 + neg_offset) = val;
        return -1;
    } else {

        if (val == date2_intro_script_tbl0) {
            return 0;
        }
        if (val == date2_intro_script_tbl1) {
            return 1;
        }
        cd_read_start(10, 0x80162000);
        idx = bg_change_tbl0;
        neg_offset = -(idx << 2);
        *(s32 *)((s32)&date2_intro_script_tbl1 + neg_offset) = val;
        return -1;
    }
}
