/* @SymbolStart: 0x8013D588 */
/* @SymbolSize: 692 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=0:a0,101:a0,122:a2 */
#include "types.h"

extern u8  ev_flag31;
extern u8  date_ride_kind;
extern u8  now_sub_sub_step;
extern u8  step_mode;
extern u8  girl8_spot24_cnt;
extern u8  girl8_spot25_cnt;
extern u8  now_girl;
extern u8  D_800EF6BF_b;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern s16 kao[];
extern s16 kao2[];
extern s16 imp[];
extern s16 ans[];
extern s32 serifu_state;

void serifu_run();
void device_cmd_send();
void k_reset();
void sub_sub_step_up();

date_basyo12_girl_step2c()
{
    if (now_girl == 7 && ev_flag31 != 0 && date_ride_kind == 1) {
        bustup_face_change(4);
        imp[0] = imp[1] = imp[2] = 4;
        ans[0] = ans[1] = ans[2] = 2;
        kao[0] = kao[1] = kao[2] = 4;
        kao2[0] = kao2[1] = kao2[2] = 3;
        now_serifu += 2;
        now_sub_sub_step += 13;
        return 0;
    }
    if ((now_girl | 0) == 8 && date_ride_kind == 3 && girl8_spot24_cnt != 0) {
        if (step_mode++ == 0) {
            now_serifu_in = 2;
        }
    }
    if ((now_girl | 0) == 8 && date_ride_kind == 4 && girl8_spot25_cnt != 0) {
        if (step_mode++ == 0) {
            now_serifu_in = 2;
        }
    }
    if (step_mode >= 11) {
        step_mode = 10;
    }
    serifu_run();
    if ((s32)D_800EF6BF_b == 8 && date_ride_kind == 3 && girl8_spot24_cnt == 0 && now_serifu_in == 3) {
        device_cmd_send(116);
        k_reset(1);
        now_serifu++;
        now_serifu_in = 0;
        serifu_state = 0;
        sub_sub_step_up();
    }
    if ((s32)D_800EF6BF_b == 8 && date_ride_kind == 4 && girl8_spot25_cnt == 0 && now_serifu_in == 3) {
        device_cmd_send(116);
        k_reset(1);
        now_serifu++;
        now_serifu_in = 0;
        serifu_state = 0;
        sub_sub_step_up();
    }
}
