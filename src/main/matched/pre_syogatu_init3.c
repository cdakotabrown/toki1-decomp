/* Likely Symbol: pre_syogatu_init (from o.bin, offset +60 bytes from 0x8006145C) */
/* Address: 0x80061420 - 0x80061480 (96 bytes) */
/* Region: GCC (OR encoding) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void text_sys_init(void);
extern void basyo_init(s32 arg);
extern void k_sub_reset_point_set(void);
extern void sndisp(s32 arg0, s32 arg1, s32 arg2);
extern void device_cmd_send(s32 arg);
extern void k_disp_start(s32 arg);
extern void sub_step_up(void);

extern u8 pre_syogatu_init3_str0;
extern u8 pre_syogatu_init3_str1;

void pre_syogatu_init3(void) {
    text_sys_init();
    basyo_init((s32)&pre_syogatu_init3_str0);
    k_sub_reset_point_set();
    sndisp((s32)&pre_syogatu_init3_str1, 0, 255);
    device_cmd_send(0x0603);
    k_disp_start(6);
    sub_step_up();
}
