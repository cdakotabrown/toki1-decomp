/* @SymbolStart: 0x80147400 */
/* @SymbolSize: 232 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=15:a0,23:v1 */
/* Address: 0x80147408 (prologue); window starts at the absorbed */
#include "types.h"

extern s32 step_counter;
extern u8 cal_cw_mode;
extern s16 cal_cw_tick;
extern s16 cal_cw_anim;
extern s16 cal_cw_frame;

extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);
extern void k_disp_inc(void);

void etc_week_cw_wait(void)
{
    s32 n;
    s32 c;

    if (step_counter == 32) {
        cal_cw_mode |= 1;
    }
    if (!(cal_cw_mode & 1) && cal_cw_frame != 0) {
        if (cal_cw_tick == 0) {
            switch (cal_cw_anim & 1) {
            case 0:
                device_cmd_send(0x601);
                break;
            case 1:
                device_cmd_send(0x602);
                break;
            }
        }
        n = cal_cw_tick;
        c = n < 17;
        cal_cw_tick = n + 1;
        if (!c) {
            sub_sub_step_up();
        }
    }
    k_disp_inc();
}
