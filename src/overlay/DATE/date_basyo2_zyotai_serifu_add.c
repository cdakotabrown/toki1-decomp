/* @SymbolSize: 188 */
/* @CompileEnv: IDO52_UGEN_DEST_TEMP=5 IDO52_EXCLUDE_REGS_WEB=2:v1 */
/* Function: date_basyo2_zyotai_serifu_add */
/* Overlay: DATE */
/* Address: 0x801391C4 */
/* Size: 188 bytes (47 instructions) */
#include "types.h"

extern s32 get_g_zyotai_s(s32);
extern s32 sub_sub_step_up(void);

extern u8 now_girl;
extern s32 serifu_no;
extern u8 date_pattern;
extern s16 now_serifu;

s32 date_basyo2_zyotai_serifu_add(void)
{
    s32 v0;
    u8 v1;
    u32 low7;

    v0 = get_g_zyotai_s(now_girl);

    if (serifu_no != 0) {
        goto extra;
    }
    v1 = now_girl;
    if (v1 == 0) {
        goto check;
    }
    if (v1 != 7) {
        goto extra;
    }
check:
    if (date_pattern != 0) {
        goto extra;
    }
    low7 = v0 & 0x7F;
    now_serifu = now_serifu + (low7 >= 2u) + (low7 >= 3u);
    goto last;

extra:
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
last:
    return sub_sub_step_up();
}
