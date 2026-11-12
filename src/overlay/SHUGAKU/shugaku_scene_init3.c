/* @PostProc: patch_stack_complete --old 32 --new 56 */
/* @PostProc: remap_stack_slots --from 24 --to 44 --from 26 --to 46 --from 28 --to 48 --from 30 --to 50 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0xc4:rt:v0:a0 0xc8:rs:v0:a0 */
/* @SymbolSize: 328 */
#include "types.h"

extern void draw2d3d(s32 a0, s32 a1);
extern void class_switch(s32 a0);
extern void prim_init(void);
extern void class_clear_all(s32 a0);
extern void set_flag_b3_high_bit(s32 a0);
extern void back_clear_switch(s32 a0);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void shared_system_state_reset(void);
extern void k_reset(s32 a0);
extern void ClearImage(s16 *a0, s32 a1, s32 a2, s32 a3);
extern void date_name_set(s32 a0);
extern void bustup_ptr_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void yazirushi_off(void);
extern void sub_sub_step_up(void);

extern u8  now_girl;
extern u8  vblank_flag;
extern volatile s32 vb_ready;
extern u8  screen_bri;
extern u8  fade_bri;
extern volatile s32 rpg_bat_arg;
extern u8  menu_x;

void shugaku_scene_init3(s32 a0v)
{
    s16 buf[4];

    if (a0v) {}
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    set_flag_b3_high_bit(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    shared_system_state_reset();
    k_reset(1);
    buf[0] = 320; buf[1] = 416; buf[2] = 320; buf[3] = 88;
    ClearImage(buf, 8, 0, 0);
    buf[0] = 704; buf[1] = 160;
    ClearImage(buf, 8, 0, 0);
    a0v = now_girl;
    vblank_flag = 0;
    vb_ready = 1;
    if ((u32)a0v >= 14U) {
        a0v = 13;
    }
    now_girl = a0v;
    date_name_set(a0v);
    bustup_ptr_init();
    screen_bri = 128;
    fade_bri = 0;
    hizuke_init();
    message_window_init();
    yazirushi_off();
    rpg_bat_arg = 255;
    menu_x = 1;
    sub_sub_step_up();
}
