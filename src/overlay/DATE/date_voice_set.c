/* @CompileFlags: -cckr */
/* @SymbolSize: 604 */
/* Address: 0x801342FC, Size: 604 bytes (151 insns).  DATE command-handler */
#include "types.h"

typedef struct {
    u8 f0;
    u8 f1;
    u8 rest[54];
} Girl;

typedef struct {
    u8 pad0[0x3E];
    u8 week;
    u8 day;
    u8 pad40[0x1D5 - 0x40];
    Girl girl[60];
    u8 pad1[0xF5F - 0x1D5 - 60 * 56];
    u8 sel;
} Save;

extern Save screen_w;

extern int ss_serifu;
extern int ss_serifu_kind;
extern int sss_sound;
extern u8 ba_string[];
extern u8 ba_string2[];
extern u8 date_voice_set_str0[];
extern u8 date_voice_set_str1[];
extern u8 date_voice_set_str2[];
extern u8 date_voice_set_str3[];
extern u8 date_serifu_stride;
extern u8 date_pattern;
extern union { int w; u8 b; } now_date_spot;
extern u8 *date_impkao_tbl_16_imp;
extern u8 *date_impkao_tbl_16_kao;
extern short kao[];
extern short imp[];
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

void date_voice_set()
{
    Save *sv;

    sv = &screen_w;
    ss_serifu = (*(int *)&date_serifu_tbl[18]);
    ss_serifu_kind = (*(int *)&date_serifu_kind_tbl[18]);
    sss_sound = (*(int *)&date_sound_tbl[18]);
    strcpy(ba_string, date_voice_set_str0);
    date_serifu_stride = 3;
    if (sv->girl[sv->sel].f1 == 0
        && (sv->week >= 97 || (sv->week == 96 && sv->day >= 3))) {
        date_pattern = 2;
        strcpy(ba_string2, date_voice_set_str1);
        now_date_spot.w = 6;
    } else if ((sv->girl[sv->sel].f0 & 1) == 0) {
        date_pattern = 0;
        strcpy(ba_string2, date_voice_set_str2);
        now_date_spot.w = 5;
    } else {
        date_pattern = 1;
        strcpy(ba_string2, date_voice_set_str3);
        now_date_spot.w = 5;
    }
    imp[0] = *(date_impkao_tbl_16_imp + sv->sel * 9 + date_pattern * 3 + 0);
    imp[1] = *(date_impkao_tbl_16_imp + sv->sel * 9 + date_pattern * 3 + 1);
    imp[2] = *(date_impkao_tbl_16_imp + sv->sel * 9 + date_pattern * 3 + 2);
    kao[0] = *(date_impkao_tbl_16_kao + sv->sel * 9 + date_pattern * 3 + 0);
    kao[1] = *(date_impkao_tbl_16_kao + sv->sel * 9 + date_pattern * 3 + 1);
    kao[2] = *(date_impkao_tbl_16_kao + sv->sel * 9 + date_pattern * 3 + 2);
    dec_bg_cache_step(17809);
    event_log_add(1, now_date_spot.b);
    sub_sub_step_up();
}
