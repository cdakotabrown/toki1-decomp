/* Address: 0x80077CDC */
/* Size: 88 bytes */
/* Region: GCC (0x80041000-0x80080FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void magazine_flag_set(s32 arg0, s32 arg1);
extern void text_sys_init(void);
extern void basyo_init(void *arg);
extern void config_screen_init(void);
extern void icon_disp_switch(s32 arg);
extern void hizuke_disp_switch(s32 arg);
extern void sub_sub_step_up(void);

extern u8 magazine_init_str0;

void magazine_init(void) {
    magazine_flag_set(0, 0);
    text_sys_init();
    basyo_init(&magazine_init_str0);
    config_screen_init();
    icon_disp_switch(0);
    hizuke_disp_switch(0);
    sub_sub_step_up();
}
