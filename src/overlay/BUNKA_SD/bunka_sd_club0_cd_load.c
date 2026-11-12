/* @SymbolStart: 0x80132000 */
/* @SymbolSize: 328 */
/* bunka_sd_club0_cd_load - BUNKA_SD overlay, Address: 0x80132008, Size: 320 bytes (80 insns) */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:10 */
#include "types.h"

extern void device_load_start();
extern s32 device_load_poll(void);
extern s32 device_ready_check(s32);
extern s32 cd_read_start2(s32, s32, s32);
extern s32 get_CD_State2(void);
extern void sub_sub_step_up(void);

extern u8 step_mode;

s32 bunka_sd_club0_cd_load(void)
{
    s32 sel;
    s32 x1;
    s32 x2;
    s32 x3;
    s32 x4;

    sel = step_mode;
    if (sel == 0) {
        device_load_start(1, 0xBF09, 0xBEF0, 0xD266, 0xD216, 0xD20E);
        x1 = (step_mode + 1) & 0xFF;
        step_mode = x1;
        return x1;
    }
    if (sel == 1) {
        if (device_load_poll() != 1) {
            goto out;
        }
        x2 = (step_mode + 1) & 0xFF;
        step_mode = x2;
        return x2;
    }
    if (sel == 2) {
        if (device_ready_check(1) != 1) {
            goto out;
        }
        x3 = (step_mode + 1) & 0xFF;
        step_mode = x3;
        return x3;
    }
    if (sel == 3) {
        cd_read_start2(82, 0x80180000, 19330);
        x4 = (step_mode + 1) & 0xFF;
        step_mode = x4;
        return x4;
    }
    if (sel == 4) {
        if (get_CD_State2() & 1) {
            sub_sub_step_up();
        }
    }
out:;
}
