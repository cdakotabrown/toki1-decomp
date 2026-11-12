/* Function: date_basyo12_serifu_skip0d */
/* Overlay: DATE */
/* @SymbolStart: 0x801396D4 */
/* @SymbolSize: 464 */
#include "types.h"

extern s32 serifu_no;
extern u8 now_girl;
extern volatile u8 now_girl_v;
extern s16 now_serifu;
extern u8 now_sub_sub_step;
extern volatile u8 now_sub_sub_step_v;
extern u8 step_mode;
extern u8 ev_flag3e;
extern u8 date_basyo12_cmd_flag;

extern void sub_sub_step_up(void);
extern void bustup_face_change(s32);
extern void serifu_run(void);

s32 date_basyo12_serifu_skip0d(void)
{
    s32 girl;
    s32 old;
    s32 cur;
    s32 nv;

    if (serifu_no == 1) {
        girl = now_girl_v;
        now_serifu = 9;
        if (girl == 7) {
            now_serifu = 10;
        } else if ((girl | 0) == 9) {
            now_serifu = 8;
        } else if (girl == 4 || girl == 8 || girl == 5 || girl == 10) {
            now_serifu = 7;
        }
        now_sub_sub_step = now_sub_sub_step + 18;
    } else {
        if (now_girl == 4 || now_girl == 8 || now_girl == 5 || now_girl == 10) {
            date_basyo12_cmd_flag = 1;
            now_sub_sub_step = now_sub_sub_step + 6;
        } else if (now_girl == 9 && ev_flag3e == 0) {
            date_basyo12_cmd_flag = 1;
            now_serifu = now_serifu + 1;
            now_sub_sub_step = now_sub_sub_step + 6;
        } else if (now_girl == 9 && ev_flag3e != 0) {
            if (step_mode == 0) {
                bustup_face_change(3);
                step_mode = 1;
            }
            old = now_sub_sub_step;
            serifu_run();
            cur = now_sub_sub_step_v;
            if (old != cur) {
                now_serifu = 8;
                cur += 17;
                now_sub_sub_step = cur &= 0xFF;
            }
        } else {
            sub_sub_step_up();
        }
    }
}
