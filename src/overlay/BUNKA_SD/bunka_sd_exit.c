/* @SymbolStart: 0x80134314 */
/* @SymbolSize: 260 */
/* bunka_sd_exit - BUNKA_SD overlay, Address: 0x8013431C, Size: 252 bytes */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:10 */
#include "types.h"

extern s32 device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);
extern void tpage_buf_clear(void);
extern void class_clear_f(void);
extern void dec_bg_reset(void);
extern void back_clear_switch(s32);
extern void hizuke_disp_switch(s32);
extern void k_speed_set(s32);
extern void goto_sub_step(s32);
extern void device_cmd_send(s32);

extern u8 step_mode;
extern s32 bunka_bukatu;
extern u8 bunka_sd_k_speed_wk;

s32 bunka_sd_exit(void)
{
    s32 sel;
    s32 n;
    s32 v1t;
    s32 v2t;

    sel = step_mode;
    if (sel == 0) {
        device_load_start(1, 0xC486, 0xC459, 0xD266, 0xD216, 0xD20E);
        v1t = (step_mode + 1) & 0xFF;
        step_mode = v1t;
        return v1t;
    }

    if (sel == 1) {
        n = device_load_poll();
        if (n != 1) {
            goto out;
        }
        v2t = (step_mode + 1) & 0xFF;
        step_mode = v2t;
        return v2t;
    }

    tpage_buf_clear();
    class_clear_f();
    if (bunka_bukatu == 1) {
        dec_bg_reset();
        back_clear_switch(1);
    }
    hizuke_disp_switch(1);
    k_speed_set((s32)bunka_sd_k_speed_wk);
    goto_sub_step(16);
    device_cmd_send(512);
out:;
}
