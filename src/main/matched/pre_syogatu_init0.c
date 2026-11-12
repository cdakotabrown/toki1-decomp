/* Likely Symbol: pre_syogatu_init1 (from o.bin, offset +188 bytes from 0x80061338) */
/* Address: 0x8006127C - 0x80061348 */
/* Size: 204 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 get_nenga_girl(void);
extern void set_dec_bri(s32 arg0);
extern void text_sys_init(void);
extern void vram_full_move(s32 arg0);
extern void class_clear_f(void);
extern void prim_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void parameter_disp_switch(s32 arg0);
extern s32 device_cmd_send(s32 arg0);
extern void icon_disp_switch(s32 arg0);
extern void tpage_buf_clear(void);
extern void cursor_init(s32 arg0);
extern void device_load_start(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void sub_sub_step_up(void);
extern void goto_step(s32 arg0);

void pre_syogatu_init0(void) {
    if (get_nenga_girl() != 0) {
        set_dec_bri(0);
        text_sys_init();
        vram_full_move(160);
        class_clear_f();
        prim_init();
        hizuke_init();
        message_window_init();
        parameter_disp_switch(0);
        device_cmd_send(193);
        icon_disp_switch(0);
        tpage_buf_clear();
        cursor_init(0);
        device_load_start(1, 0xc4f3, 0xc4d5, 0xd2a2, 0xd272, 0xd26c);
        sub_sub_step_up();
    } else {
        set_dec_bri(0);
        goto_step(83);
    }
}
