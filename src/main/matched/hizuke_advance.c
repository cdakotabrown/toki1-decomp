/* Function: hizuke_advance */
/* Address: 0x80072410 (window 0x800723F8..0x80072653, the 6-word hoisted */
/* Size: 612 bytes (153 instructions) incl. the head */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x800723F8 */
/* @SymbolSize: 612 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 now_youbi;
extern u8 now_day;
extern volatile u8 now_month;
extern u8 now_year;
extern u16 pass_days;
extern u8 schedule_cursor;
extern u8 day_flag1;
extern u8 byouki_state;
extern u8 schedule_buf;

extern void quarter_flag_set(void);
extern void season_last_date_spot_clear(void);
extern void day_flag_set(s32);
extern void reserved_date_sort(void);

void hizuke_advance(void) {
    s32 dayOfWeek;
    s32 month;

    dayOfWeek = now_youbi + 1;
    now_youbi = (u8)dayOfWeek;

    if (now_youbi == 7) {
        now_youbi = 0;
    }

    now_day = now_day + 1;
    month = now_month;

    switch (month - 1) {
    case 0:
    case 2:
    case 4:
    case 6:
    case 7:
    case 9:
    case 11:
        if ((u32)now_day < 32) {
            break;
        }
        now_day = 1;
        now_month = month + 1;
        quarter_flag_set();
        month = now_month;
        break;
    case 3:
    case 5:
    case 8:
    case 10:
        if ((u32)now_day < 31) {
            break;
        }
        now_day = 1;
        now_month = month + 1;
        quarter_flag_set();
        month = now_month;
        break;
    case 1:
        if (now_year == 96) {
            if ((u32)now_day >= 30) {
                now_day = 1;
                now_month = month + 1;
                quarter_flag_set();
            }
            month = now_month;
            break;
        }
        if ((u32)now_day < 29) {
            break;
        }
        now_day = 1;
        now_month = month + 1;
        quarter_flag_set();
        month = now_month;
        break;
    }

    if ((u32)month >= 13) {
        now_year = now_year + 1;
        month = 1;
    }

    pass_days = pass_days + 1;
    now_month = month;

    if (((u32)month % 3) == 0) {
        season_last_date_spot_clear();
    }

    if (schedule_cursor != 0) {
        day_flag_set(0);
        day_flag1 = 0;
    } else {
        day_flag_set(0);
        day_flag1 = 1;
    }

    dayOfWeek = now_youbi;
    if (dayOfWeek == 6) {
        if ((*(u16 *)&byouki_state & 1) != 0) {
            if (schedule_buf != 64) {
                byouki_state = byouki_state & 0xFFFE;
            }
        }
    }

    reserved_date_sort();
}
