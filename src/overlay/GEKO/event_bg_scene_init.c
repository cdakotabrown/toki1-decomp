/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 31 --to 43 */
/* event_bg_scene_init -- GEKO overlay, Address: 0x801348D8, Size: 236 bytes (59 insns) */
#include "types.h"

extern void draw2d3d(s32 a0, s32 a1);
extern void class_switch(s32 a0);
extern void prim_init(void);
extern void class_clear_all(s32 a0);
extern void back_clear_switch(s32 a0);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void shared_system_state_reset(void);
extern void strcpy(s32 *a0, s32 *a1);
extern s32 k_reset(s32 a0);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void yazirushi_off(void);
extern void dec_bg_cache_step(s32 a0);
extern void event_log_add(s32 a0, s32 a1);
extern void sub_sub_step_up(void);

extern s32 ba_string;
extern s32 event_bg_scene_init_str0;
extern s32 pick_girl;

extern u8 bg_change_tbl2;
extern u8 vblank_flag;
extern s32 vb_ready;
extern u8 screen_bri;
extern u8 fade_bri;

void event_bg_scene_init(void)
{
    volatile u8 tmp;
    s32 wide;

    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    wide = pick_girl;
    tmp = (u8)wide;
    shared_system_state_reset();
    pick_girl = (s32)tmp;
    strcpy(&ba_string, &event_bg_scene_init_str0);
    k_reset(1);
    vblank_flag = 0;
    vb_ready = 1;
    screen_bri = 128;
    fade_bri = 0;
    bg_change_tbl2 = 0;
    hizuke_init();
    message_window_init();
    yazirushi_off();
    dec_bg_cache_step(16708);
    event_log_add(13, 37);
    sub_sub_step_up();
}
