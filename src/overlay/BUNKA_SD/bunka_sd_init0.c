/* bunka_sd_init0 -- BUNKA_SD overlay, Address: 0x80134260, Size: 180 bytes (45 insns) */
#include "types.h"

extern void shared_system_state_reset(void);
extern void class_clear_f(void);
extern void k_reset(s32);
extern void tpage_buf_clear(void);
extern void back_clear_switch(s32);
extern void draw2d3d(s32 a0, s32 a1);
extern void class_switch(s32 a0);
extern void hizuke_disp_switch(s32);
extern void prim_init(void);
extern s32 get_k_speed(void);
extern void sub_sub_step_up(void);

extern u8 back_clear_r;
extern u8 back_clear_b;
extern u8 back_clear_g;
extern s32 bunka_bukatu;
extern s32 bunka_sd_k_speed_wk;

void bunka_sd_init0(void)
{
    s32 ret;

    shared_system_state_reset();
    class_clear_f();
    k_reset(1);
    tpage_buf_clear();

    back_clear_r = 0;
    back_clear_b = 0;
    back_clear_g = 0;

    if (bunka_bukatu == 1)
    {
        back_clear_switch(0);
    }
    else
    {
        back_clear_switch(1);
    }
    draw2d3d(1, 0);
    class_switch(1);
    hizuke_disp_switch(0);
    prim_init();
    ret = get_k_speed();
    bunka_sd_k_speed_wk = ret;
    sub_sub_step_up();
}
