/* @SymbolStart: 0x80139344 */
/* @SymbolSize: 480 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=0:a0,49:v1,77:v1 */
/* Address: 0x80139344 (head) / 0x8013934C, 480 bytes (120 instructions). */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 get_CD_l_com(void);
extern void k_reset(s32);
extern s32 check_end_k(void);
extern void shougatu_date_serifu_run(void);

extern u8 voice_level;
extern u8 now_girl;
extern s32 step_counter;
extern s16 sound_seek_time;
extern u8 yazirushi_cw_disp;
extern s32 serifu_state;
extern s32 serifu_voice;
extern s32 kuchi_stop;
extern s16 shougatu_date_now_serifu;
extern s16 shougatu_date_now_serifu_in;

s32 shougatu_serifu_voice_wait(void)
{
    s32 mode;
    s32 t;
    s32 n;

    if (shougatu_date_now_serifu_in == 2) {
        now_girl = 7;
    }
    shougatu_date_serifu_run();
    mode = shougatu_date_now_serifu_in;
    if (mode == 2) {
        yazirushi_cw_disp &= 0xFF7F;
    }
    if (mode == 3) {
        kuchi_stop = 1;
        yazirushi_cw_disp &= 0xFF7F;
    } else {
        kuchi_stop = 0;
    }
    if (mode == 2) {
        if (check_end_k() != 0) {
            t = voice_level;
            if (t == 0 || (t != 0 && get_CD_l_com() == 9)) {
                k_reset(1);
                serifu_state = 0;
                serifu_voice = 0;
                yazirushi_cw_disp &= 0xFF7F;
                sound_seek_time = 0;
                step_counter = 0;
                return;
            }
        }
    }
    mode = shougatu_date_now_serifu_in;
    if ((u32)step_counter++ >= 26U) {
        if (mode == 3) {
        if (check_end_k() != 0) {
        t = voice_level;
        if (t == 0 || (t != 0 && get_CD_l_com() == 9)) {
            kuchi_stop = 1;
            shougatu_date_now_serifu = shougatu_date_now_serifu + 1;
            serifu_state = 0;
            serifu_voice = 0;
            yazirushi_cw_disp &= 0xFF7F;
            sound_seek_time = 0;
            shougatu_date_now_serifu_in = 0;
            sub_sub_step_up();
        }
        }
        }
    }
}
