/* @CompileFlags: -cckr */
/* Function: date_basyo0_serifu_select */
/* Overlay: DATE */
#include "types.h"

typedef struct {
    s16 v[11];
} DateTbl;

typedef struct {
    u32 b0 : 5;
    u32 f5 : 1;
    u32 b6 : 8;
    u32 f14 : 1;
    u32 b15 : 17;
    u32 kind : 4;
    u32 rank : 5;
    u32 restBits : 23;
    u8 rest[48];
} Girl;

typedef struct {
    u8 pad0[0x3E];
    u8 week;
    u8 day;
    u8 x40;
    u8 pad41[0x1C8 - 0x41];
    Girl girl[60];
    u8 pad1[0xF5F - 0x1C8 - 60 * 56];
    u8 sel;
} Save;

extern Save screen_w;
extern DateTbl date_basyo0_serifu_select_tbl0;
extern DateTbl date_basyo0_serifu_select_tbl1;
extern u32 frame_counter;
extern int serifu_no;
extern short date_now_serifu_basyo0;
extern short date_now_serifu_basyo0_in;
extern u8 date_birth_near_flag;
extern u8 date_basyo0_serifu_flag;

date_basyo0_serifu_select()
{
    int r2;
    u8 b107;
    DateTbl s1;
    DateTbl s2;
    u8 b59;
    int d;
    Save *sv;

    sv = &screen_w;
    s1 = date_basyo0_serifu_select_tbl0;
    s2 = date_basyo0_serifu_select_tbl1;

    if (sv->week == 97 && sv->day >= 6) {
        s2.v[3] = s2.v[3] + 1;
    } else if (sv->week == 98) {
        s2.v[3] = s2.v[3] + 1;
    }

    date_now_serifu_basyo0 = serifu_no;
    date_now_serifu_basyo0_in = frame_counter %
        (unsigned)((serifu_no == 0) * s1.v[sv->sel] + s2.v[sv->sel] * (serifu_no == 1));
    b59 = 0;
    date_birth_near_flag = 0;
    if (sv->day == sv->girl[sv->sel].kind) {
        if (sv->x40 < sv->girl[sv->sel].rank) {
            date_birth_near_flag = 1;
        }
    } else {
        d = sv->girl[sv->sel].kind - sv->day + 12;
        if (d % 12U == 1) {
            date_birth_near_flag = 1;
        }
    }

    date_basyo0_serifu_flag = 0;
    if ((sv->week == 97 && sv->day >= 3) || sv->week == 98) {
        if (rand() % 2) {
            date_basyo0_serifu_flag = 1;
        }
    }

    b107 = get_g_zyotai_s(sv->sel) & 0x7F;
    r2 = get_g_zyotai_h(sv->sel);
    if (b107 == 0 && (r2 & 0xFF) == 0) {
        if (sv->girl[sv->sel].f5 == 0) {
            sv->girl[sv->sel].f5 = 1;
            b59 = 1;
        }
    }

    if (b59 != 0 && serifu_no == 0) {
        date_now_serifu_basyo0 = 4;
        date_now_serifu_basyo0_in = 0;
    } else if (date_birth_near_flag != 0 && serifu_no == 0) {
        date_now_serifu_basyo0 = 2;
        date_now_serifu_basyo0_in = sv->week - 95;
        if (sv->sel == 6) {
            if (b107 < 2 && (r2 & 0xFF) < 2U) {
                date_now_serifu_basyo0_in = 1;
            } else {
                date_now_serifu_basyo0_in = 0;
            }
        }
    } else if (date_basyo0_serifu_flag != 0 && serifu_no == 0) {
        date_now_serifu_basyo0 = 3;
        date_now_serifu_basyo0_in = 0;
    }

    if (sv->sel == 5 && date_now_serifu_basyo0 == 0 && date_now_serifu_basyo0_in == 3) {
        sv->girl[5].f14 = 1;
    }
    sub_sub_step_up();
}
