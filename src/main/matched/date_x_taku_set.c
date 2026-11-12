/* Likely Symbol: date_x_taku_set (from o.bin, offset +224 bytes from 0x80085F70) */
/* Address: 0x80086050 */
/* Size: 160 bytes (40 instructions) */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80086000-0x800BFFFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void draw2d3d(s32, s32);
extern void class_switch(s32);
extern void prim_init(void);
extern void class_clear_f(void);
extern void back_clear_switch(s32);
extern void text_sys_init(void);
extern void func_8004863C(void);
extern void tpage_buf_clear(void);

extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern u8 screen_bri;
extern u8 vblank_flag;

void date_x_taku_set(s32 arg0, s32 arg1)
{
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_f();
    back_clear_switch(arg0);
    back_clear_r = 0;
    back_clear_g = 0;
    back_clear_b = 0;
    screen_bri = 128;
    text_sys_init();
    vblank_flag = 0;
    if (arg1 != 0) {
        func_8004863C();
    } else {
        tpage_buf_clear();
    }
}
