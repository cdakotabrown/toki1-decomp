/* Address: 0x80078690 - 0x800786F8 (104 bytes, 26 instructions) */
/* Region: GCC (OR encoding) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void mes_window_switch(s32 arg0);
extern void text_sys_init(void);
extern void x_taku_string_set(void *arg0, void *arg1, s32 arg2, s32 arg3);
extern void x_taku_menu_set(s32 arg0, s32 arg1);
extern void k_disp_start(s32 arg0);
extern void sub_sub_step_up(void);

extern u8 holiday_club_init_str0;
extern u8 holiday_club_init_str1;
extern u8 menu_csr;

void holiday_club_init(void) {
    mes_window_switch(0);
    text_sys_init();
    x_taku_string_set(&holiday_club_init_str0, &holiday_club_init_str1, 0, 2);
    x_taku_menu_set(1, 2);
    menu_csr = 0;
    k_disp_start(2);
    sub_sub_step_up();
}
