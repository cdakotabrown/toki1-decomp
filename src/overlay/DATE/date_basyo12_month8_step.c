/* @SymbolStart: 0x8013A86C */
/* @SymbolSize: 148 */
/* date_basyo12_month8_step - DATE overlay, Address: 0x8013A884, Size: 124 bytes (31 insns) */
#include "types.h"
extern s32 serifu_no;
extern s32 serifu_no_bak;
extern u8 now_month;

extern void sub_sub_step_up(void);
extern s32 get_g_zyotai_h(s32);
extern void bustup_face_change(s32);
extern u8 now_girl;
extern u8 now_sub_sub_step;
extern u8 date_aug_flag;
extern s16 date_now_serifu_basyo12;
extern s16 date_now_serifu_basyo12_in;

void date_basyo12_month8_step(void)
{
    s32 t7_in;

    serifu_no_bak = serifu_no;
    t7_in = now_month;

if (t7_in == 8) {
        date_aug_flag = 1;
        date_now_serifu_basyo12 = 0;
        date_now_serifu_basyo12_in = 0;
        bustup_face_change(get_g_zyotai_h(now_girl) & 0xFF);
        sub_sub_step_up();
        return;
    }
    sub_sub_step_up();
    now_sub_sub_step = now_sub_sub_step + 16;
}
