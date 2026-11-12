/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 IDO52_FORCE_WEB_COLOR=0:a3 */
/* @SymbolStart: 0x80149B40 */
/* @SymbolSize: 380 */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 get_CD_State2();
extern s32 k_sub_reset();
extern s32 k_sub_disp_start();
extern s32 gnsx();
extern s32 sndisp();
extern s32 set_dec_bri();
extern s32 dec_bg_reset();
extern s32 indexed_byte_lsb_set();
extern s32 value_cache_find_or_load();
extern s32 dec_bg_init();
extern s32 dec_bg_show();
extern s32 parameter_disp_switch();
extern s32 hizuke_init();
extern u8 p_sei[];
extern u8 step_mode;
extern s32 dec_bg_handle;
extern u8 etc_siken_bg_load_str0[];

s32 etc_siken_bg_load()
{
    s32 ret;
    s32 n;

    ret = step_mode;
    switch (step_mode) {
    case 0:
        k_sub_reset();
        gnsx(p_sei);
        sndisp(etc_siken_bg_load_str0, 0, 31);
        k_sub_disp_start(2);
        ret = value_cache_find_or_load(17979, 1);
        n = ret;
        if (ret != -1) {
            if (ret == 0 || ret == 1) {
                step_mode = step_mode + 2;
                indexed_byte_lsb_set(1, n);
                ret = set_dec_bri(128);
            }
        } else {
            step_mode = step_mode + 1;
        }
        break;
    case 1:
        ret = get_CD_State2();
        if ((ret & 1) != 0) {
            ret = dec_bg_init(1);
            indexed_byte_lsb_set(1, ret);
            if (dec_bg_handle == -1) {
                dec_bg_reset();
                ret = value_cache_find_or_load(17979, 1);
            } else {
                ret = set_dec_bri(128);
                step_mode = step_mode + 1;
            }
        }
        break;
    case 2:
        parameter_disp_switch(0);
        dec_bg_show(1);
        set_dec_bri(128);
        hizuke_init();
        ret = sub_sub_step_up();
        break;
    }
    return ret;
}
