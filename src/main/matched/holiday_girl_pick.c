/* Address: 0x80077068 */
/* Size: 852 bytes (213 instructions) */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=0:a0 IDO52_UGEN_TEMP_REG=1:a0 IDO52_UGEN_TEMP_REG_INPLACE=occ:1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 get_nf(void);
extern s32 get_param_top_girl(s32 arg0);

extern s32 work_fuku;
extern u8  schedule_cursor;
extern u8  schedule_kind[];
extern u8  holiday_girl_pick_tbl2[];
extern u8  holiday_girl_pick_tbl3[];
extern u16 basyo_pack;
extern u8  holiday_girl_pick_tbl0[11][2];
extern u8  holiday_girl_pick_tbl1[11][2];

void holiday_girl_pick(void)
{
    s32 val;
    s32 v1;
    u8  idx;
    u8  v0;

    val = 255;
    work_fuku = val;
    if (get_nf() == 1) {
        idx = schedule_cursor;
        v0 = schedule_kind[idx * 4];
        if (v0 == 4) {
            val = 255;
        } else {
            val = holiday_girl_pick_tbl2[v0];
        }
    } else {
        idx = schedule_cursor;
        v0 = schedule_kind[idx * 4];
        if (v0 == 4) {
            val = 255;
        } else {
            val = holiday_girl_pick_tbl3[v0];
        }
    }
    work_fuku = val;
    if (val != 255) {
        return;
    }

    v1 = get_param_top_girl(4);
    if (v1 != 0) {
        v1 = 1;
    }
    if (get_nf() == 1) {
        switch ((u32)(basyo_pack >> 12)) {
        case 0:
            val = holiday_girl_pick_tbl0[0][v1];
            work_fuku = val;
            break;
        case 1:
            val = holiday_girl_pick_tbl0[1][v1];
            work_fuku = val;
            break;
        case 2:
            val = holiday_girl_pick_tbl0[2][v1];
            work_fuku = val;
            break;
        case 3:
            val = holiday_girl_pick_tbl0[3][v1];
            work_fuku = val;
            break;
        case 4:
            val = holiday_girl_pick_tbl0[4][v1];
            work_fuku = val;
            break;
        case 5:
            val = holiday_girl_pick_tbl0[5][v1];
            work_fuku = val;
            break;
        case 6:
            val = holiday_girl_pick_tbl0[6][v1];
            work_fuku = val;
            break;
        case 7:
            val = holiday_girl_pick_tbl0[7][v1];
            work_fuku = val;
            break;
        case 8:
            val = holiday_girl_pick_tbl0[8][v1];
            work_fuku = val;
            break;
        case 9:
            val = holiday_girl_pick_tbl0[9][v1];
            work_fuku = val;
            break;
        case 10:
            val = holiday_girl_pick_tbl0[10][v1];
            work_fuku = val;
            break;
        }
    } else {
        switch ((u32)(basyo_pack >> 12)) {
        case 0:
            val = holiday_girl_pick_tbl1[0][v1];
            work_fuku = val;
            break;
        case 1:
            val = holiday_girl_pick_tbl1[1][v1];
            work_fuku = val;
            break;
        case 2:
            val = holiday_girl_pick_tbl1[2][v1];
            work_fuku = val;
            break;
        case 3:
            val = holiday_girl_pick_tbl1[3][v1];
            work_fuku = val;
            break;
        case 4:
            val = holiday_girl_pick_tbl1[4][v1];
            work_fuku = val;
            break;
        case 5:
            val = holiday_girl_pick_tbl1[5][v1];
            work_fuku = val;
            break;
        case 6:
            val = holiday_girl_pick_tbl1[6][v1];
            work_fuku = val;
            break;
        case 7:
            val = holiday_girl_pick_tbl1[7][v1];
            work_fuku = val;
            break;
        case 8:
            val = holiday_girl_pick_tbl1[8][v1];
            work_fuku = val;
            break;
        case 9:
            val = holiday_girl_pick_tbl1[9][v1];
            work_fuku = val;
            break;
        case 10:
            val = holiday_girl_pick_tbl1[10][v1];
            work_fuku = val;
            break;
        }
    }
}
