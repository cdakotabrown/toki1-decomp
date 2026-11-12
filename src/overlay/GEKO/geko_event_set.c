/* @CompileFlags: -cckr */
/* @SymbolSize: 1172 */
/* @PostProc: swap_branch_operands --offsets 0x5c,0x70,0x80,0x88,0xe8,0xf4,0xfc,0x104,0x144,0x14c,0x154,0x18c,0x194,0x19c,0x1d0,0x1e0,0x220,0x228,0x230,0x238,0x26c,0x28c,0x2a8,0x2c0,0x2c8,0x2f4,0x30c,0x314,0x340,0x358,0x360,0x38c,0x394,0x39c */
#include "types.h"

typedef struct {
    u8 b[21];
} DateFlagTable;

extern u8 now_girl;
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 basyo_flag2;
extern u8 basyo_flag3;
extern u8 basyo_flag5;
extern u8 basyo_flag7;
extern u8 basyo_flag8;
extern u8 basyo_flaga;
extern u8 basyo_flagb;
extern u8 basyo_flagc;
extern u8 basyo_flagd;
extern u8 basyo_flage;
extern u8 basyo_flagf;
extern u8 girl8_spot6_cnt;
extern u8 girl8_spot9_cnt;
extern u8 girl8_spot18_cnt;
extern u8 girl8_spot19_cnt;
extern u8 ev_flag0c;
extern u8 ev_flag18;
extern u8 ev_flag1e;
extern u8 ev_flag2f;
extern u8 ev_flag39;
extern u8 ev_flag47;
extern u8 geko_pick_no;
extern DateFlagTable geko_event_set_tbl;

void geko_event_set()
{
    u8 chr;
    u8 year;
    u8 month;
    u8 day;
    DateFlagTable buf;

    buf = geko_event_set_tbl;
    chr = now_girl;
    if (chr == 1 && now_year == 96 &&
        (now_month == 12 || now_month == 1 || (now_month == 2 && now_day < 21)) &&
        ev_flag0c == 0) {
        geko_pick_no = buf.b[14];
        basyo_flage = 1;
    }
    year = now_year;
    month = now_month;
    day = now_day;
    if (chr == 3 && year == 97 && (month == 9 || month == 10 || (month == 11 && day < 21)) &&
        ev_flag18 == 0) {
        geko_pick_no = buf.b[7];
        basyo_flag7 = 1;
    }
    if (chr == 4 && (month == 9 || (month == 10 && day < 21)) && year >= 96 &&
        ev_flag1e == 0) {
        geko_pick_no = buf.b[13];
        basyo_flagd = 1;
    }
    if (chr == 7 && (month == 3 || month == 4 || (month == 5 && day < 21)) &&
        ev_flag2f == 0) {
        geko_pick_no = buf.b[2];
        basyo_flag2 = 1;
    }
    if (chr == 10 && year >= 96 && month == 10 && day < 21 && ev_flag47 == 0) {
        geko_pick_no = buf.b[13];
        basyo_flagd = 1;
    }
    if (chr == 8) {
        if (((year == 95 && month == 12) || (year == 96 && month == 1) ||
             (year == 96 && month == 2 && day < 21)) && girl8_spot6_cnt == 0) {
            geko_pick_no = buf.b[3];
            basyo_flag3 = 1;
        }
        if (year == 96 && girl8_spot9_cnt == 0 && (month == 6 || month == 7)) {
            geko_pick_no = buf.b[5];
            basyo_flag5 = 1;
        }
        if (year == 96 && girl8_spot18_cnt == 0 &&
            (month == 9 || month == 10 || (month == 11 && day < 21))) {
            geko_pick_no = buf.b[10];
            basyo_flaga = 1;
        }
        if (year == 97 && girl8_spot19_cnt == 0 &&
            (month == 3 || month == 4 || (month == 5 && day < 21))) {
            geko_pick_no = buf.b[11];
            basyo_flagb = 1;
        }
        if (year == 97 && ev_flag39 == 0 &&
            (month == 9 || month == 10 || (month == 11 && day < 21))) {
            geko_pick_no = buf.b[15];
            basyo_flagf = 1;
        }
        if (year == 97 && (month == 12 || month == 1 || (month == 2 && day < 11))) {
            geko_pick_no = buf.b[12];
            basyo_flagc = 1;
        }
        if ((geko_week_end_get(0) == 95 && geko_week_end_get(1) == 12 && geko_week_end_get(2) == 10) ||
            (geko_week_end_get(0) == 96 && geko_week_end_get(1) == 12 && geko_week_end_get(2) == 8) ||
            (geko_week_end_get(0) == 97 && geko_week_end_get(1) == 12 && geko_week_end_get(2) == 7)) {
            geko_pick_no = buf.b[8];
            basyo_flag8 = 1;
        }
    }
}
