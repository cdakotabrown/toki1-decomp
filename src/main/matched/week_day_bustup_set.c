/* Address: 0x80073D64 (the recorded 0x80073D8C is the prologue; the 40-byte */
/* Size: 1288 bytes (322 instructions) */
/* Region: Game (IDO 5.2) */
/* @SymbolStart: 0x80073D64 */
/* @SymbolSize: 1288 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:8 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 pad0;
    u8 subtype;
    u8 pad2;
    u8 type;
} CharRec;

typedef struct {
    u8 pad0[1692];
    CharRec chars[1];
} CharTable;

extern CharTable screen_w;
extern u8 schedule_cursor;
extern u16 basyo_pack;
extern s32 work_fuku;
extern u8 week_day_bustup_set_tbl0[];
extern u8 holiday_girl_pick_tbl0[];

extern s32 get_nf(void);

void week_day_bustup_set(void)
{
    u8 type;
    s32 v1;

    work_fuku = 255;
    type = screen_w.chars[schedule_cursor].type;
    if (type != 0) {
        v1 = 1;
    } else {
        v1 = 0;
    }
    if (screen_w.chars[schedule_cursor].subtype == 3 && type == 5) {
        v1 = 2;
    }
    if (get_nf() == 1) {
        switch (screen_w.chars[schedule_cursor].subtype) {
        case 0: work_fuku = week_day_bustup_set_tbl0[v1 + 0]; break;
        case 1: work_fuku = week_day_bustup_set_tbl0[v1 + 2]; break;
        case 2: work_fuku = week_day_bustup_set_tbl0[v1 + 4]; break;
        case 3: work_fuku = week_day_bustup_set_tbl0[v1 + 6]; break;
        case 4: work_fuku = 255; break;
        case 5: work_fuku = week_day_bustup_set_tbl0[v1 + 9]; break;
        case 6: work_fuku = week_day_bustup_set_tbl0[v1 + 11]; break;
        case 7: work_fuku = week_day_bustup_set_tbl0[13]; break;
        }
    } else {
        switch (screen_w.chars[schedule_cursor].subtype) {
        case 0: work_fuku = week_day_bustup_set_tbl0[v1 + 16]; break;
        case 1: work_fuku = week_day_bustup_set_tbl0[v1 + 18]; break;
        case 2: work_fuku = week_day_bustup_set_tbl0[v1 + 20]; break;
        case 3: work_fuku = week_day_bustup_set_tbl0[v1 + 22]; break;
        case 4: work_fuku = 255; break;
        case 5: work_fuku = week_day_bustup_set_tbl0[v1 + 25]; break;
        case 6: work_fuku = week_day_bustup_set_tbl0[v1 + 27]; break;
        case 7: work_fuku = week_day_bustup_set_tbl0[29]; break;
        }
    }
    if (work_fuku != 255) {
        return;
    }
    if (get_nf() == 1) {
        switch (basyo_pack >> 12) {
        case 0: work_fuku = holiday_girl_pick_tbl0[v1 + 0]; break;
        case 1: work_fuku = holiday_girl_pick_tbl0[v1 + 2]; break;
        case 2: work_fuku = holiday_girl_pick_tbl0[v1 + 4]; break;
        case 3: work_fuku = holiday_girl_pick_tbl0[v1 + 6]; break;
        case 4: work_fuku = holiday_girl_pick_tbl0[v1 + 8]; break;
        case 5: work_fuku = holiday_girl_pick_tbl0[v1 + 10]; break;
        case 6: work_fuku = holiday_girl_pick_tbl0[v1 + 12]; break;
        case 7: work_fuku = holiday_girl_pick_tbl0[v1 + 14]; break;
        case 8: work_fuku = holiday_girl_pick_tbl0[v1 + 16]; break;
        case 9: work_fuku = holiday_girl_pick_tbl0[v1 + 18]; break;
        case 10: work_fuku = holiday_girl_pick_tbl0[v1 + 20]; break;
        }
    } else {
        switch (basyo_pack >> 12) {
        case 0: work_fuku = holiday_girl_pick_tbl0[v1 + 24]; break;
        case 1: work_fuku = holiday_girl_pick_tbl0[v1 + 26]; break;
        case 2: work_fuku = holiday_girl_pick_tbl0[v1 + 28]; break;
        case 3: work_fuku = holiday_girl_pick_tbl0[v1 + 30]; break;
        case 4: work_fuku = holiday_girl_pick_tbl0[v1 + 32]; break;
        case 5: work_fuku = holiday_girl_pick_tbl0[v1 + 34]; break;
        case 6: work_fuku = holiday_girl_pick_tbl0[v1 + 36]; break;
        case 7: work_fuku = holiday_girl_pick_tbl0[v1 + 38]; break;
        case 8: work_fuku = holiday_girl_pick_tbl0[v1 + 40]; break;
        case 9: work_fuku = holiday_girl_pick_tbl0[v1 + 42]; break;
        case 10: work_fuku = holiday_girl_pick_tbl0[v1 + 44]; break;
        }
    }
}
