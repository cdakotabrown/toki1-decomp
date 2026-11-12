/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8013a960 */
/* @SymbolSize: 476 */
/* Address: 0x8013A960 (cross-prologue head) / 0x8013A968 (body) */
/* Size: 476 bytes (119 insns, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 step_mode;
extern s32 step_counter;
extern s32 sched_icon;
extern s32 step34_load_tbl0[];
extern u8 tel_home_ring_init_str0;
extern u8 tel_home_ring_init_str1;

extern void text_sys_init(void);
extern void basyo_init(s32);
extern void k_sub_disp_start(s32);
extern void parameter_show_init(void);
extern void message_window_init(void);
extern void hizuke_init(void);
extern void icon_init(void);
extern void icon_disp_switch(s32);
extern void icon_schedule_set(void);
extern void icon_can_use_set(s32, s32);
extern void parameter_disp_switch(s32);
extern void k_sub_reset_point_set(void);
extern void class_clear_f(void);
extern s32 get_season(void);
extern s32 value_cache_find_or_load(s32, s32);
extern void indexed_byte_lsb_set(s32, s32);
extern s32 set_dec_bri(s32);
extern s32 get_CD_State2(void);
extern s32 dec_bg_init(s32);
extern void device_cmd_send(s32);
extern void sndi(s32, s32, s32);
extern s32 sub_sub_step_up(void);

s32 tel_home_ring_init(void)
{
    s32 ret;
    s32 result;

    ret = step_mode;
    switch (step_mode) {
        case 0:
            text_sys_init();
            basyo_init((s32)&tel_home_ring_init_str0);
            k_sub_disp_start(1);
            parameter_show_init();
            parameter_show_init();
            message_window_init();
            hizuke_init();
            icon_init();
            icon_disp_switch(1);
            icon_schedule_set();
            icon_can_use_set(0, 0);
            icon_can_use_set(sched_icon & 0xF, 1);
            parameter_disp_switch(1);
            k_sub_reset_point_set();
            k_sub_disp_start(1);
            class_clear_f();
            result = get_season();
            ret = value_cache_find_or_load(step34_load_tbl0[result], 0);
            result = ret;
            if (ret != -1) {
                if (ret != 0) {
                    if (ret != 1) {
                        return;
                    }
                }
                indexed_byte_lsb_set(0, result);
                ret = set_dec_bri(128);
                step_mode = 2;
                break;
            }
            step_mode = step_mode + 1;
            break;
        case 1:
            ret = get_CD_State2();
            if ((ret & 1) == 0) {
                return;
            }
            result = dec_bg_init(0);
            indexed_byte_lsb_set(0, result);
            ret = set_dec_bri(128);
            step_mode = 2;
            break;
        case 2:
            if ((u32)step_counter >= 76U) {
                step_mode = step_mode + 1;
            }
            break;
        case 3:
            device_cmd_send(1289);
            sndi((s32)&tel_home_ring_init_str1, 0, 47);
            ret = sub_sub_step_up();
            break;
    }
    return ret;
}
