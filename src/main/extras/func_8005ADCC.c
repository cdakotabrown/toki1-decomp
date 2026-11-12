/* Address: 0x8005ADCC */
/* Size: 108 bytes (27 instructions) */
/* Compiler: GCC-like (0x80040000-0x80080000 region) */
#include "types.h"

extern void draw2d3d(s32, s32);
extern void class_switch(s32);
extern void prim_init(void);
extern void class_clear_f(void);
extern void set_dec_bri(s32);
extern void text_sys_init(void);
extern void k_disp_start(s32);
extern void sub_sub_step_up(void);

extern u8 vblank_flag;
extern u8 attract_wait;

void func_8005ADCC(void) {
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_f();
    vblank_flag = 0;
    attract_wait = 0;
    set_dec_bri(0);
    text_sys_init();
    k_disp_start(2);
    sub_sub_step_up();
}
