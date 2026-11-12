/* @CompileFlags: -cckr */
/* @SymbolSize: 620 */
/* Address: 0x8013521C, Size: 620 bytes (155 insns).  DATE command-handler */
#include "types.h"

typedef struct {
    u8 f0;
    u8 rest[55];
} Girl;

typedef struct {
    u8 pad0[0x1DF];
    Girl girl[60];
    u8 pad1[0xF5F - 0x1DF - 60 * 56];
    u8 sel;
} Save;

extern Save screen_w;

extern int ss_serifu;
extern int ss_serifu_kind;
extern int sss_sound;
extern u8 ba_string[];
extern u8 ba_string2[];
extern u8 date_voice_set4_str0[];
extern u8 date_voice_set4_str1[];
extern u8 date_voice_set4_str2[];
extern u8 date_voice_set4_str3[];
extern u8 date_serifu_stride;
extern u8 date_pattern;
extern short now_serifu;
extern union { int w; u8 b; } now_date_spot;
extern u8 *date_impkao_tbl_17_imp;
extern u8 *date_impkao_tbl_17_kao;
extern short kao[];
extern short imp[];
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

void date_library_voice_set()
{
    Save *sv;

    sv = &screen_w;
    ss_serifu = (*(int *)&date_serifu_tbl[19]);
    ss_serifu_kind = (*(int *)&date_serifu_kind_tbl[19]);
    sss_sound = (*(int *)&date_sound_tbl[19]);
    strcpy(ba_string, date_voice_set4_str0);
    date_serifu_stride = 3;
    if ((sv->girl[sv->sel].f0 & 1) == 0) {
        date_pattern = 0;
        strcpy(ba_string2, date_voice_set4_str1);
    } else {
        date_pattern = 1;
        strcpy(ba_string2, date_voice_set4_str2);
        if (sv->sel == 7) {
            now_serifu = 3;
        } else if (sv->sel == 10) {
            now_serifu = 1;
        }
    }
    if (sv->sel == 8 && sv->girl[sv->sel].f0 != 0) {
        date_pattern = 1;
        strcpy(ba_string2, date_voice_set4_str3);
    }
    now_date_spot.w = 15;
    imp[0] = *(date_impkao_tbl_17_imp + sv->sel * 6 + date_pattern * 3 + 0);
    imp[1] = *(date_impkao_tbl_17_imp + sv->sel * 6 + date_pattern * 3 + 1);
    imp[2] = *(date_impkao_tbl_17_imp + sv->sel * 6 + date_pattern * 3 + 2);
    kao[0] = *(date_impkao_tbl_17_kao + sv->sel * 6 + date_pattern * 3 + 0);
    kao[1] = *(date_impkao_tbl_17_kao + sv->sel * 6 + date_pattern * 3 + 1);
    kao[2] = *(date_impkao_tbl_17_kao + sv->sel * 6 + date_pattern * 3 + 2);
    dec_bg_cache_step(17732);
    event_log_add(1, now_date_spot.b);
    sub_sub_step_up();
}
