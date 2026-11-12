/* @CompileFlags: -cckr */
/* @SymbolSize: 552 */
/* Address: 0x80135488, Size: 552 bytes (138 insns).  DATE command-handler */
#include "types.h"

typedef struct {
    u8 f0;
    u8 rest[55];
} Girl;

typedef struct {
    u8 pad0[0x3E];
    u8 week;
    u8 day;
    u8 pad40[0x1E0 - 0x40];
    Girl girl[60];
    u8 pad1[0xF5F - 0x1E0 - 60 * 56];
    u8 sel;
} Save;

extern Save screen_w;

extern int ss_serifu;
extern int ss_serifu_kind;
extern int sss_sound;
extern u8 ba_string[];
extern u8 ba_string2[];
extern u8 date_voice_set5_str0[];
extern u8 date_voice_set5_str1[];
extern u8 date_voice_set5_str2[];
extern u8 date_serifu_stride;
extern u8 date_late96_flag;
extern u8 date_pattern;
extern union { int w; u8 b; } now_date_spot;
extern u8 *date_impkao_tbl_5_imp;
extern u8 *date_impkao_tbl_5_kao;
extern short kao[];
extern short imp[];
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

void date_game_center_voice_set()
{
    Save *sv;

    sv = &screen_w;
    ss_serifu = (*(int *)&date_serifu_tbl[5]);
    ss_serifu_kind = (*(int *)&date_serifu_kind_tbl[5]);
    sss_sound = (*(int *)&date_sound_tbl[5]);
    strcpy(ba_string, date_voice_set5_str0);
    date_serifu_stride = 0;
    if ((sv->girl[sv->sel].f0 & 1) == 0) {
        date_pattern = 0;
        strcpy(ba_string2, date_voice_set5_str1);
    } else {
        date_pattern = 1;
        strcpy(ba_string2, date_voice_set5_str2);
    }
    now_date_spot.w = 16;
    if (sv->week >= 97 || (sv->week >= 96 && sv->day >= 9)) {
        date_late96_flag = 1;
    } else {
        date_late96_flag = 0;
    }
    imp[0] = *(date_impkao_tbl_5_imp + sv->sel * 12 + date_pattern * 3 + 0);
    imp[1] = *(date_impkao_tbl_5_imp + sv->sel * 12 + date_pattern * 3 + 1);
    imp[2] = *(date_impkao_tbl_5_imp + sv->sel * 12 + date_pattern * 3 + 2);
    kao[0] = *(date_impkao_tbl_5_kao + sv->sel * 12 + date_pattern * 3 + 0);
    kao[1] = *(date_impkao_tbl_5_kao + sv->sel * 12 + date_pattern * 3 + 1);
    kao[2] = *(date_impkao_tbl_5_kao + sv->sel * 12 + date_pattern * 3 + 2);
    dec_bg_cache_step(17622);
    event_log_add(1, now_date_spot.b);
    sub_sub_step_up();
}
