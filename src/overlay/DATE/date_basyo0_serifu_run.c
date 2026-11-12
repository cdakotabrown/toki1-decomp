/* @SymbolStart: 0x8013B820 */
/* @SymbolSize: 140 */
/* Function: date_basyo0_serifu_run */
/* Overlay: DATE */
/* Address: 0x8013B820 (cross-prologue head) / 0x8013B864 (body) */
/* Size: 140 bytes (35 instructions) */
#include "types.h"

extern s32 serifu_main(s32, s32, s32);
extern void sub_sub_step_up(void);

extern u8 date_now_serifu_basyo0[];
extern u8 date_now_serifu_basyo0_in[];

extern s32 now_s;
extern s32 now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;

extern u8 serifu_no;
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

void date_basyo0_serifu_run(void)
{
    now_s = (s32)date_now_serifu_basyo0;
    now_s_in = (s32)date_now_serifu_basyo0_in;
    ss_s = date_serifu_tbl[25];
    ss_s_kind = date_serifu_kind_tbl[25];
    sss_so = date_sound_tbl[25];
    if (serifu_main((s32)serifu_no, 0, 0) != 0) {
        sub_sub_step_up();
    }
}
