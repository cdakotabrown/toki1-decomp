/* @SymbolStart: 0x801394F4 */
/* @SymbolSize: 148 */
#include "types.h"

extern void date_name_set(s32 a0);
extern void sub_sub_step_up(void);

extern s16  now_serifu_shugaku;
extern s32  ss_serifu_shugaku;
extern s32  ss_serifu_shugaku_kind;
extern s32  sss_sound_shugaku;
extern u8   D_800D07EC_b;
extern u8   now_girl;
extern u8   now_sub_sub_step;


extern s32 serifu_no;
extern s32 shugaku_serifu_tbl[];
extern s32 shugaku_serifu_kind_tbl[];
extern s32 shugaku_sound_tbl[];

void shugaku_sel_girl13_set(void)
{
    s32 t7, t8, t9, t0, t1, t2;

    if (serifu_no == 0) goto lbl_80139570;
    now_serifu_shugaku = 0;
    ss_serifu_shugaku = shugaku_serifu_tbl[12];
    ss_serifu_shugaku_kind = shugaku_serifu_kind_tbl[12];
    sss_sound_shugaku = shugaku_sound_tbl[12];
    D_800D07EC_b = 1;
    now_girl = 13;
    date_name_set(13);
    now_sub_sub_step = 51;
    goto lbl_80139578;
lbl_80139570:
    sub_sub_step_up();
lbl_80139578:
    return;
}
