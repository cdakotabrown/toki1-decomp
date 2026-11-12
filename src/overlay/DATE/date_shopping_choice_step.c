/* @SymbolSize: 488 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8 flag;
    u8 rest[55];
} Girl;

typedef struct {
    u8 pad0[0x1D9];
    Girl girl[60];
    u8 pad1[0xF5F - 0x1D9 - 60 * 56];
    u8 sel;
} Save;

extern Save screen_w;

extern int ss_serifu;
extern int ss_serifu_kind;
extern int sss_sound;
extern u8 ba_string[];
extern u8 ba_string2[];
extern u8 date_shopping_choice_step_str0[];
extern u8 date_shopping_choice_step_str1[];
extern u8 date_serifu_stride;
extern u8 date_pattern;
extern union { int w; u8 b; } now_date_spot;
extern u8 *date_impkao_tbl_11_imp;
extern u8 *date_impkao_tbl_11_kao;
extern s16 kao[];
extern s16 imp[];
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

date_shopping_choice_step()
{
    Save *sv;

    sv = &screen_w;
    ss_serifu = (*(int *)&date_serifu_tbl[13]);
    ss_serifu_kind = (*(int *)&date_serifu_kind_tbl[13]);
    sss_sound = (*(int *)&date_sound_tbl[13]);
    strcpy(ba_string, date_shopping_choice_step_str0);
    strcpy(ba_string2, date_shopping_choice_step_str1);
    date_serifu_stride = 3;
    if (sv->sel == 7) {
        date_serifu_stride = 5;
    }
    if ((sv->girl[sv->sel].flag & 1) == 0) {
        date_pattern = 0;
    } else {
        date_pattern = 1;
    }
    now_date_spot.w = 9;
    (*(short *)&imp[0]) = *(date_impkao_tbl_11_imp + sv->sel * 6 + date_pattern * 3 + 0);
    (*(short *)&imp[1]) = *(date_impkao_tbl_11_imp + sv->sel * 6 + date_pattern * 3 + 1);
    (*(short *)&imp[2]) = *(date_impkao_tbl_11_imp + sv->sel * 6 + date_pattern * 3 + 2);
    (*(short *)&kao[0]) = *(date_impkao_tbl_11_kao + sv->sel * 6 + date_pattern * 3 + 0);
    (*(short *)&kao[1]) = *(date_impkao_tbl_11_kao + sv->sel * 6 + date_pattern * 3 + 1);
    (*(short *)&kao[2]) = *(date_impkao_tbl_11_kao + sv->sel * 6 + date_pattern * 3 + 2);
    dec_bg_cache_step(17875);
    event_log_add(1, now_date_spot.b);
    sub_sub_step_up();
}
