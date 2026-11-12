/* Function: date_girl3_mode2_init */
/* Overlay: DATE, Address: 0x801517C8, Size: 240 bytes (60 insns) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: scheduler_normalizer --block-aware */
#include "types.h"

extern s32 rand();
extern s32 param_clamp();
extern s32 sub_sub_step_up();

extern u8 date_bg_ptn[];
extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s16 girl3_tokimeki;
extern s16 girl3_yuukou;
extern s16 girl3_syoushin;
extern u8 sys_flags;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

s32 date_girl3_mode2_init(void)
{
    s16 i;
    s32 v0;

    for (i = 0; i < 12; i++) {
        v0 = rand();
        date_bg_ptn[i] = (u8)(v0 % 4);
    }

    date_ss_serifuev = date_serifuev_tbl[25];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[25];
    date_sss_soundev = date_soundev_tbl[25];
    girl3_tokimeki = girl3_tokimeki + 2;
    girl3_yuukou = girl3_yuukou + 1;
    girl3_syoushin = girl3_syoushin - 20;
    param_clamp();
    sys_flags = sys_flags | 1;
    v0 = sub_sub_step_up();
    return v0;
}
