/* @SymbolStart: 0x8013D83C */
/* @SymbolSize: 312 */
/* @PostProc: swap_branch_operands --offsets 0x78 */
/* Function: date_basyo12_girl_skip2d */
/* Overlay: DATE */
#include "types.h"

extern void sub_sub_step_up(void);

extern u8 now_girl;
extern u8 date_ride_kind;
extern u8 ev_flag09;
extern u8 now_month;
extern u8 now_sub_sub_step;
extern u8 girl4_spot24_cnt;
extern u8 girl8_spot23_cnt;

void date_basyo12_girl_skip2d(void)
{
    s32 v0 = now_girl;
    s32 v1;

    if (v0 == 1) {
        v1 = date_ride_kind;
        if (v1 == 3) {
            v1 = ev_flag09;
            if (v1 != 0) goto lbl_D8A4;
            v0 = now_month;
            if ((u32)v0 < 3U) goto lbl_D894;
            if ((u32)v0 < 6U) goto lbl_D8A4;
        lbl_D894:
            now_sub_sub_step = now_sub_sub_step + 13;
        lbl_D8A4:
            if (v1 != 1) goto lbl_D95C;
            now_sub_sub_step = now_sub_sub_step + 13;
            goto lbl_D95C;
        }
    }

    v1 = date_ride_kind;
    if (v0 == 4 && v1 == 3) {
        if (girl4_spot24_cnt != 0) {
            now_sub_sub_step = now_sub_sub_step + 7;
            goto lbl_D95C;
        }
    }

    if (v0 == 7) {
        if (v1 == 1) {
            now_sub_sub_step = now_sub_sub_step + 7;
            goto lbl_D95C;
        }
    }

    if (v0 == 8 && v1 == 2) {
        if (girl8_spot23_cnt != 0) {
            now_sub_sub_step = now_sub_sub_step + 7;
        }
    }

lbl_D95C:
    sub_sub_step_up();
}
