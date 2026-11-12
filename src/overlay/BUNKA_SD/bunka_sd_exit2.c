/* @SymbolStart: 0x801388D0 */
/* @SymbolSize: 268 */
/* bunka_sd_exit2 - BUNKA_SD overlay, Address: 0x801388D8, Size: 260 bytes */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:14 */
#include "types.h"

extern s32 get_CD_State(void);
extern void device_load_start();
extern s32 device_load_poll(void);
extern void tpage_buf_clear(void);
extern void class_clear_f(void);
extern void hizuke_disp_switch(s32);
extern void goto_sub_step(s32);
extern void device_cmd_send(s32);

extern u8 step_mode;

s32 bunka_sd_exit2(void)
{
    s32 sel;
    s32 v1t;
    s32 v2t;
    s32 v3t;

    sel = step_mode;

    if (sel == 0) {
        if (get_CD_State() != 2) {
            goto out;
        }
        v1t = (step_mode + 1) & 0xFF;
        step_mode = v1t;
        return v1t;
    }
    if (sel == 1) {
        device_load_start(1, 0xC486, 0xC459, 0xD266, 0xD216, 0xD20E);
        v2t = (step_mode + 1) & 0xFF;
        step_mode = v2t;
        return v2t;
    }
    if (sel == 2) {
        if (device_load_poll() != 1) {
            goto out;
        }
        v3t = (step_mode + 1) & 0xFF;
        step_mode = v3t;
        return v3t;
    }
    tpage_buf_clear();
    class_clear_f();
    hizuke_disp_switch(1);
    goto_sub_step(16);
    device_cmd_send(512);
out:;
}
