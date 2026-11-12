/* Function: pre_xmas_init */
/* Address: 0x80060EFC */
/* Size: 224 bytes (56 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 now_sub_sub_step;

extern void text_sys_init(void);
extern void class_clear_f(void);
extern void prim_init(void);
extern void basyo_init2(void* arg0);
extern void gnsx(void* arg0);
extern void parameter_show_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void parameter_disp_switch(s32 arg0);
extern void icon_disp_switch(s32 arg0);
extern void tpage_buf_clear(void);
extern void sndisp(void* arg0, s32 arg1, s32 arg2);
extern void k_disp_start(s32 arg0);
extern void cursor_init(s32 arg0);
extern s32 step34_load(void);
extern s32 step34_init3(void);

extern u8 pre_xmas_init_str0[];
extern u8 pre_xmas_init_str1[];
extern u8 p_sei;

s32 pre_xmas_init(void) {
    s32 swVar = now_sub_sub_step;
    s32 result;

    result = swVar;
    switch (swVar) {
    case 0:
        text_sys_init();
        class_clear_f();
        prim_init();
        basyo_init2(pre_xmas_init_str0);
        gnsx(&p_sei);
        parameter_show_init();
        parameter_show_init();
        hizuke_init();
        message_window_init();
        parameter_disp_switch(1);
        icon_disp_switch(0);
        tpage_buf_clear();
        sndisp(pre_xmas_init_str1, 0, 31);
        k_disp_start(6);
        cursor_init(0);
        result = step34_load();
        goto done;
    case 1:
        result = step34_init3();
        break;
    }

done:
    return result;
}
