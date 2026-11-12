/* Function: week_day_bustup_set */
/* Address: 0x80073D8C */
/* Size: 972 bytes (243 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* CONFIRMED UNMATCHABLE (2026-03-28): Added to config/c_unmatchable.txt. */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script fix_func_80073D8C.py */
#include "types.h"

extern u8 schedule_cursor;
extern u8 screen_w[];
extern u16 basyo_pack;
extern s32 work_fuku;

extern u8 week_day_bustup_set_tbl0[];
extern u8 D_800BCDC2[];
extern u8 D_800BCDC4[];
extern u8 D_800BCDC6[];
extern u8 D_800BCDC9[];
extern u8 D_800BCDCB[];
extern u8 D_800BCDCD;

extern u8 D_800BCDD0[];
extern u8 D_800BCDD2[];
extern u8 D_800BCDD4[];
extern u8 D_800BCDD6[];
extern u8 D_800BCDD9[];
extern u8 D_800BCDDB[];
extern u8 D_800BCDDD;

extern u8 holiday_girl_pick_tbl0[];
extern u8 D_800BCD92[];
extern u8 D_800BCD94[];
extern u8 D_800BCD96[];
extern u8 D_800BCD98[];
extern u8 D_800BCD9A[];
extern u8 D_800BCD9C[];
extern u8 D_800BCD9E[];
extern u8 D_800BCDA0[];
extern u8 D_800BCDA2[];
extern u8 D_800BCDA4[];

extern u8 holiday_girl_pick_tbl1[];
extern u8 D_800BCDAA[];
extern u8 D_800BCDAC[];
extern u8 D_800BCDAE[];
extern u8 D_800BCDB0[];
extern u8 D_800BCDB2[];
extern u8 D_800BCDB4[];
extern u8 D_800BCDB6[];
extern u8 D_800BCDB8[];
extern u8 D_800BCDBA[];
extern u8 D_800BCDBC[];

extern s32 func_80066BB4(void);

void week_day_bustup_set(s32 direction) {
    s32 v1;
    s32 result;
    u8 idx;
    u8 type_val;
    u32 key2;

    if (direction != 0) {
        v1 = 1;
    } else {
        v1 = 0;
    }

    if (((u8 *)direction)[1693] == 3 && direction == 5) {
        v1 = 2;
    }

    result = func_80066BB4();

    if (result == 1) {
        idx = schedule_cursor;
        type_val = screen_w[(s32)idx * 4 + 1693];
        switch (type_val) {
        case 0:
            work_fuku = week_day_bustup_set_tbl0[v1];
            break;
        case 1:
            work_fuku = D_800BCDC2[v1];
            break;
        case 2:
            work_fuku = D_800BCDC4[v1];
            break;
        case 3:
            work_fuku = D_800BCDC6[v1];
            break;
        case 4:
            work_fuku = 255;
            break;
        case 5:
            work_fuku = D_800BCDC9[v1];
            break;
        case 6:
            work_fuku = D_800BCDCB[v1];
            break;
        case 7:
            work_fuku = D_800BCDCD;
            break;
        default:
            break;
        }
    } else {
        idx = schedule_cursor;
        type_val = screen_w[(s32)idx * 4 + 1693];
        switch (type_val) {
        case 0:
            work_fuku = D_800BCDD0[v1];
            break;
        case 1:
            work_fuku = D_800BCDD2[v1];
            break;
        case 2:
            work_fuku = D_800BCDD4[v1];
            break;
        case 3:
            work_fuku = D_800BCDD6[v1];
            break;
        case 4:
            work_fuku = 255;
            break;
        case 5:
            work_fuku = D_800BCDD9[v1];
            break;
        case 6:
            work_fuku = D_800BCDDB[v1];
            break;
        case 7:
            work_fuku = D_800BCDDD;
            break;
        default:
            break;
        }
    }

    if (work_fuku != 255) {
        return;
    }

    result = func_80066BB4();

    if (result == 1) {
        key2 = (u32)basyo_pack >> 12;
        switch (key2) {
        case 0:
            work_fuku = holiday_girl_pick_tbl0[v1];
            break;
        case 1:
            work_fuku = D_800BCD92[v1];
            break;
        case 2:
            work_fuku = D_800BCD94[v1];
            break;
        case 3:
            work_fuku = D_800BCD96[v1];
            break;
        case 4:
            work_fuku = D_800BCD98[v1];
            break;
        case 5:
            work_fuku = D_800BCD9A[v1];
            break;
        case 6:
            work_fuku = D_800BCD9C[v1];
            break;
        case 7:
            work_fuku = D_800BCD9E[v1];
            break;
        case 8:
            work_fuku = D_800BCDA0[v1];
            break;
        case 9:
            work_fuku = D_800BCDA2[v1];
            break;
        case 10:
            work_fuku = D_800BCDA4[v1];
            break;
        default:
            break;
        }
    } else {
        key2 = (u32)basyo_pack >> 12;
        switch (key2) {
        case 0:
            work_fuku = holiday_girl_pick_tbl1[v1];
            break;
        case 1:
            work_fuku = D_800BCDAA[v1];
            break;
        case 2:
            work_fuku = D_800BCDAC[v1];
            break;
        case 3:
            work_fuku = D_800BCDAE[v1];
            break;
        case 4:
            work_fuku = D_800BCDB0[v1];
            break;
        case 5:
            work_fuku = D_800BCDB2[v1];
            break;
        case 6:
            work_fuku = D_800BCDB4[v1];
            break;
        case 7:
            work_fuku = D_800BCDB6[v1];
            break;
        case 8:
            work_fuku = D_800BCDB8[v1];
            break;
        case 9:
            work_fuku = D_800BCDBA[v1];
            break;
        case 10:
            work_fuku = D_800BCDBC[v1];
            break;
        default:
            break;
        }
    }
}
