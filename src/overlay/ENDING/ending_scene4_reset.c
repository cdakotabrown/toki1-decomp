/* ending_scene4_reset - ENDING overlay, Address: 0x80135EEC, Size: 128 bytes (32 instructions) */
#include "types.h"

extern void prim_init(void);
extern void class_clear_all(s32);
extern void func_8004863C(void);
extern void k_speed_set(s32);
extern void text_sys_init(void);
extern void k_reset(s32);
extern void vram_rect_clear(s32);
extern void sub_sub_step_up(void);

extern u8 ending_scene4_on;
extern s32 tpage_buf[];

void ending_scene4_reset(void)
{
    s32 one;

    prim_init();
    class_clear_all(0);
    func_8004863C();

    one = 1;
    tpage_buf[28] = one;
    tpage_buf[29] = 1;
    tpage_buf[30] = 1;

    k_speed_set(16);
    text_sys_init();
    k_reset(1);
    vram_rect_clear(3);

    ending_scene4_on = 0;
    sub_sub_step_up();
}
