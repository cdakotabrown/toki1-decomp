/* Likely Symbol: holiday_tel_init (from o.bin, offset +28 bytes from 0x80078050) */
/* Address: 0x8007806C */
/* Size: 120 bytes (30 instructions) */
/* Compiler: IDO 5.2 -O2 -G0 -mips1 + patch_frame_size 24->40 */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "common.h"

extern void text_sys_init(void);
extern void class_clear_f(void);
extern void mes_window_switch(int arg0);
extern void icon_disp_switch(int arg0);
extern void telephone_choice_menu_init(void);
extern void k_sub_reset_point_set(void);
extern void gnsx(void *arg0);
extern void sndi(void *arg0, int arg1, int arg2);
extern void k_sub_disp_start(int arg0);
extern void sub_sub_step_up(void);

extern u8 p_sei;
extern u8 holiday_tel_init_str0;

void holiday_tel_init(void) {
    text_sys_init();
    class_clear_f();
    mes_window_switch(0);
    icon_disp_switch(0);
    telephone_choice_menu_init();
    k_sub_reset_point_set();
    gnsx(&p_sei);
    sndi(&holiday_tel_init_str0, 0, 0x1F);
    k_sub_disp_start(2);
    sub_sub_step_up();
}
