/* Function: func_80057780 */
/* Likely Symbol: func_80057794 */
/* Address: 0x80057884 */
/* Size: 292 bytes (73 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_80057884.py */
#include "types.h"

extern s32 dec_bg_handle;
extern u8 date_effect_init_tbl0[];

extern void dec_env_set(u32, u32, u32, u32);
extern s32 decdct(s32, s32, s32, s32);

s32 func_80057780(s32 arg0)
{
    s32 val;
    s32 v1;
    s32 diff;
    s32 ret;

    ret = 0;
    dec_bg_handle = ret;
    dec_env_set(0x80167000, 0x80162000, 0x80162000, arg0);

    if (arg0 != 0) {
        goto block1;
    }
    val = date_effect_init_tbl0[arg0];
    v1 = 1;
    diff = v1 - val;
    if (diff == 0) {
        goto block0_case0;
    }
    if (diff == v1) {
        goto block0_case1;
    }
    return diff;
block0_case0:
    dec_bg_handle = decdct(704, 160, 320, 88);
    return 0;
block0_case1:
    dec_bg_handle = decdct(320, 416, 320, 88);
    return 1;
block1:
    if (arg0 != 1) {
        goto epilogue;
    }
    val = date_effect_init_tbl0[arg0];
    v1 = 1;
    diff = v1 - val;
    if (diff == 0) {
        goto block1_case0;
    }
    if (diff == v1) {
        goto block1_case1;
    }
    goto epilogue;
block1_case0:
    dec_bg_handle = decdct(704, 0, 320, 160);
    return 0;
block1_case1:
    dec_bg_handle = decdct(320, 256, 320, 160);
    return 1;
epilogue:
    return diff;
}
