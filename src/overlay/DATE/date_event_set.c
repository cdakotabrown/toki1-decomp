/* @CompileFlags: -cckr */
/* @SymbolSize: 1320 */
/* Address: 0x80134CF4, Size: 1320 bytes (330 insns).  Reached only through */
#include "types.h"

typedef struct {
    u8 flag;
    u8 f1;
    u8 f2;
    u8 f3;
    u8 f4;
    u8 f5;
    u8 rest[50];
} Girl;

typedef struct {
    u8 pad0[0x3E];
    u8 week;
    u8 day;
    u8 pad40[0x1D9 - 0x40];
    Girl girl[60];
    u8 pad1[0xF5F - 0x1D9 - 60 * 56];
    u8 sel;
} Save;

extern Save screen_w;

extern int date_serifu_tbl;
extern int date_serifu_kind_tbl;
extern int date_sound_tbl;
extern int ss_serifu;
extern int ss_serifu_kind;
extern int sss_sound;
extern u8 ba_string[];
extern u8 ba_string2[];
extern u8 date_event_set_str0[];
extern u8 date_event_set_str1[];
extern u8 date_event_set_str2[];
extern u8 date_event_set_str3[];
extern u8 date_event_set_str4[];
extern u8 date_event_set_str5[];
extern u8 date_serifu_stride;
extern u8 date_spot12_flag;
extern u8 date_spot13_flag;
extern u8 date_spot14_flag;
extern u8 date_pattern;
extern union { int w; u8 b; } now_date_spot;
extern short now_serifu;
extern u8 *date_impkao_tbl;
extern u8 *date_impkao_tbl_0_kao;
extern short kao[];
extern short imp[];

date_event_set()
{
    Save *sv;

    sv = &screen_w;
    ss_serifu = date_serifu_tbl;
    ss_serifu_kind = date_serifu_kind_tbl;
    sss_sound = date_sound_tbl;
    strcpy(ba_string, date_event_set_str0);
    date_serifu_stride = 3;
    date_spot14_flag = 0;
    date_spot13_flag = 0;
    date_spot12_flag = 0;
    if (sv->week == 97 && sv->day >= 9 && sv->day < 12) {
        date_pattern = 2;
        date_spot12_flag = 1;
        strcpy(ba_string2, date_event_set_str1);
        if (sv->girl[sv->sel].f3 != 0 && sv->sel != 3) {
            date_pattern = 5;
        }
        now_date_spot.w = 12;
        if (sv->sel == 7) {
            now_serifu = 2;
        }
    } else if (sv->week == 97 && sv->day >= 3 && sv->day < 6) {
        date_pattern = 3;
        date_spot13_flag = 1;
        strcpy(ba_string2, date_event_set_str2);
        if (sv->girl[sv->sel].f4 != 0) {
            date_pattern = 5;
        }
        now_date_spot.w = 13;
        if (sv->sel == 7) {
            now_serifu = 4;
        }
    } else if (sv->week == 95 && sv->day >= 9 && sv->day < 12) {
        date_pattern = 4;
        date_spot14_flag = 1;
        strcpy(ba_string2, date_event_set_str3);
        if (sv->girl[sv->sel].f5 != 0) {
            date_pattern = 5;
        }
        now_date_spot.w = 14;
        if (sv->sel == 7) {
            now_serifu = 4;
        }
    } else if ((sv->girl[sv->sel].f2 & 1) == 0) {
        date_pattern = 0;
        strcpy(ba_string2, date_event_set_str4);
        now_date_spot.w = 11;
    } else {
        date_pattern = 1;
        strcpy(ba_string2, date_event_set_str5);
        now_date_spot.w = 11;
        if (sv->sel == 7) {
            now_serifu = 2;
        }
    }
    imp[0] = *(date_impkao_tbl + sv->sel * 15 + date_pattern * 3 + 0);
    imp[1] = *(date_impkao_tbl + sv->sel * 15 + date_pattern * 3 + 1);
    imp[2] = *(date_impkao_tbl + sv->sel * 15 + date_pattern * 3 + 2);
    kao[0] = *(date_impkao_tbl_0_kao + sv->sel * 15 + date_pattern * 3 + 0);
    kao[1] = *(date_impkao_tbl_0_kao + sv->sel * 15 + date_pattern * 3 + 1);
    kao[2] = *(date_impkao_tbl_0_kao + sv->sel * 15 + date_pattern * 3 + 2);
    if (date_pattern == 5) {
        switch (sv->sel) {
        default:
            kao[0] = kao[1] = kao[2] = 3;
            imp[0] = imp[1] = imp[2] = 3;
            break;
        case 1:
        case 3:
        case 7:
        case 8:
            kao[0] = kao[1] = kao[2] = 4;
            imp[0] = imp[1] = imp[2] = 4;
            break;
        }
        if (sv->sel == 3 && date_spot12_flag != 0) {
            imp[0] = imp[1] = imp[2] = 3;
        }
        if (sv->sel == 10) {
            now_serifu++;
        }
        if (sv->sel == 7) {
            now_serifu = 4;
        }
    }
    dec_bg_cache_step(17649);
    event_log_add(1, now_date_spot.b);
    return sub_sub_step_up();
}
