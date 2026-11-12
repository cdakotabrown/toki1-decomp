/* Address: 0x8006080C */
/* Size: 64 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void mes_window_switch(s32 arg0);
extern void icon_disp_switch(s32 arg0);
extern void text_sys_init(void);
extern void join_club_class_init(void);
extern void sub_sub_step_up(void);

void join_club_init(void) {
    mes_window_switch(0);
    icon_disp_switch(0);
    text_sys_init();
    join_club_class_init();
    sub_sub_step_up();
}
