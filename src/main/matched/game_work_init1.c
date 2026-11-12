/* Function: game_work_init1 */
/* Address: 0x80041AA4 */
/* Size: 280 bytes (70 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x80041A70 */
/* @SymbolSize: 332 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 season_no;
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 now_youbi;
extern u16 pass_days;
extern u8 cal_day_kind[];
extern u8 cal_kind_apr2;
extern u8 cal_kind_apr9;
extern u8 cal_kind_apr16;
extern u8 cal_kind_apr23;
extern u8 cal_kind_apr29;
extern u8 cal_kind_apr30;
extern u8 cal_cell_tbl;
extern u8 cal_cell_tbl_4;
extern u8 cal_days_may;
extern u8 cal_kind_may3;
extern u8 cal_kind_may4;
extern u8 cal_kind_may5;
extern u8 cal_kind_may7;
extern u8 cal_kind_may14;
extern u8 cal_kind_may21;
extern u8 cal_kind_may28;
extern u8 cal_days_jun;
extern u8 bukatu_rec[];
extern u8 mouse_pad_st[];

extern void bzero(u8 *, s32);

void game_work_init1(void) {
    season_no = 0;
    now_year = 95;
    now_month = 4;
    now_day = 4;
    now_youbi = 1;
    pass_days = 0;
    bzero(cal_day_kind, 128);
    bzero(bukatu_rec, 48);
    bzero(mouse_pad_st, 4);
    cal_day_kind[0] = 30;
    cal_cell_tbl = 5;
    cal_kind_apr2 = 1;
    cal_kind_apr9 = 1;
    cal_kind_apr16 = 1;
    cal_kind_apr23 = 1;
    cal_kind_apr29 = 2;
    cal_kind_apr30 = 1;
    cal_cell_tbl_4 = 1;
    cal_days_may = 31;
    cal_days_jun = 0;
    cal_kind_may7 = 1;
    cal_kind_may14 = 1;
    cal_kind_may21 = 1;
    cal_kind_may28 = 1;
    cal_kind_may3 = 2;
    cal_kind_may4 = 2;
    cal_kind_may5 = 2;
}
