/* @SymbolStart: 0x8013D3F4 */
/* @SymbolSize: 404 */
/* @PostProc: swap_branch_operands --offsets 0xc8 */
/* Function: date_basyo12_girl_step2b */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern void bustup_face_change(s32);
extern void date_trans_set(void);

extern u8 now_girl;
extern u8 date_ride_kind;
extern u8 ev_flag09;
extern u8 now_month;
extern u8 girl8_spot23_cnt;
extern u8 girl4_spot24_cnt;
extern s16 now_serifu;
extern s16 kao[];
extern s16 imp[];

s32 date_basyo12_girl_step2b(void)
{
    s32 girl = now_girl;
    s32 loc;
    s32 sub;
    u32 evt;

    if (girl == 1) {
        loc = date_ride_kind;
        if (loc == 3) {
            sub = ev_flag09;
            if (sub != 0) goto lbl_D4AC;
            evt = now_month;
            if (evt < 3U) goto lbl_D454;
            if (evt < 6U) goto lbl_D4AC;
        lbl_D454:
            now_serifu = now_serifu + 1;
            imp[0] = imp[1] = imp[2] = 3;
            kao[0] = kao[1] = kao[2] = 3;
            date_trans_set();
            bustup_face_change(3);
            return 0;
        lbl_D4AC:
            if (sub == 1) {
                imp[0] = imp[1] = imp[2] = 4;
                kao[0] = kao[1] = kao[2] = 4;
                date_trans_set();
                bustup_face_change(4);
                return 0;
            }
            goto lbl_D570;
        }
    }

    loc = date_ride_kind;
    if (girl == 8 && loc == 2) {
        if (girl8_spot23_cnt != 0) {
            now_serifu = now_serifu + 3;
            goto lbl_D570;
        }
    }

    if (girl == 4 && loc == 3) {
        if (girl4_spot24_cnt != 0) {
            now_serifu = now_serifu + 3;
        }
    }

lbl_D570:
    return sub_sub_step_up();
}
