/* @SymbolStart: 0x80137B40 */
/* @SymbolSize: 364 */
/* bunka_sd_club1_y96_cd_load - BUNKA_SD overlay, Address: 0x80137B48, Size: 356 bytes */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:10 */
#include "types.h"

extern void device_load_start();
extern s32 device_load_poll(void);
extern s32 device_ready_check(s32);
extern s32 cd_read_start2(s32, s32, s32);
extern s32 get_CD_State2(void);
extern void sub_sub_step_up(void);

extern u8 step_mode;
extern u8 now_girl;
extern u8 bunka_sd_cue_line_tbl[];
extern u8 bunka_sd_cue_xa_tbl[];
extern u8 *bunka_sd_cue_line_ptr;
extern u8 *bunka_sd_cue_xa_ptr;

s32 bunka_sd_club1_y96_cd_load(void)
{
    s32 sel;
    s32 v1t;
    s32 v2t;
    s32 v3t;
    s32 v4t;

    sel = step_mode;

    if (sel == 0) {
        device_load_start(1, 0xBF09, 0xBEF0, 0xC9EA, 0xC9A5, 0xC996);
        v1t = (step_mode + 1) & 0xFF;
        step_mode = v1t;
        return v1t;
    }
    if (sel == 1) {
        if (device_load_poll() != 1) {
            goto out;
        }
        v2t = (step_mode + 1) & 0xFF;
        step_mode = v2t;
        return v2t;
    }
    if (sel == 2) {
        if (device_ready_check(1) != 1) {
            goto out;
        }
        v3t = (step_mode + 1) & 0xFF;
        step_mode = v3t;
        return v3t;
    }
    if (sel == 3) {
        cd_read_start2(210, 0x80180000, 19656);
        v4t = (step_mode + 1) & 0xFF;
        step_mode = v4t;
        return v4t;
    }
    if (sel == 4) {
        if (get_CD_State2() & 1) {
            bunka_sd_cue_line_ptr = bunka_sd_cue_line_tbl;
            bunka_sd_cue_xa_ptr = bunka_sd_cue_xa_tbl;
            now_girl = 14;
            sub_sub_step_up();
        }
    }
out:;
}
