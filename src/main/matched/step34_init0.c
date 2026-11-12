/* Debug Symbol: step34_load (from o.bin, high match) */
/* Address: 0x8005A5CC */
/* Size: 108 bytes (27 instructions) */
/* Region: GCC (0x80041000-0x80080FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void draw2d3d(s32 a0, s32 a1);
extern void class_switch(s32 a0);
extern void prim_init(void);
extern void class_clear_f(void);
extern void set_dec_bri(s32 a0);
extern void text_sys_init(void);
extern void k_disp_start(s32 a0);
extern void sub_sub_step_up(void);

extern u8 vblank_flag;
extern u8 attract_wait;

void step34_init0(void) {
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
