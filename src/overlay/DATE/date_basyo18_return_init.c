/* @SymbolSize: 456 */
/* @PostProc: patch_stack_complete --old 40 --new 56 */
/* @PostProc: remap_stack_slots --from 31 --to 43 --from 32 --to 44 --from 34 --to 46 --from 36 --to 48 --from 38 --to 50 */
/* Address: 0x8013A4CC, Size: 456 bytes (114 insns) */
#include "types.h"

extern void draw2d3d(s32 a0, s32 a1);
extern void class_switch(s32 a0);
extern void prim_init(void);
extern void class_clear_all(s32 a0);
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
extern void device_cmd_send(s32 a0);
extern void dec_bg_cache_step(s32 a0);
extern void sub_sub_step_up(void);

extern s32 now_date_spot;
extern u8 fade_bri;
extern s32 serifu_no;
extern u8 vblank_flag;
extern volatile s32 vb_ready;
extern volatile u8 now_girl;
extern u8 screen_bri;
extern s16 kao[];
extern s16 kao2[];
extern s16 imp[];
extern s16 ans[];

void date_basyo18_return_init(void)
{
    s16 rect[4];
    u8 tmp;

    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    tmp = (u8)now_date_spot;
    shared_system_state_reset();
    now_date_spot = tmp;
    k_reset(1);

    rect[0] = 320;
    rect[1] = 416;
    rect[2] = 320;
    rect[3] = 88;
    ClearImage(rect, 8, 0, 0);
    rect[0] = 704;
    rect[1] = 160;
    ClearImage(rect, 8, 0, 0);

    vblank_flag = 0;
    vb_ready = 1;
    date_name_set(now_girl);
    bustup_ptr_init();
    screen_bri = 128;
    fade_bri = 0;
    hizuke_init();
    message_window_init();
    yazirushi_off();
    device_cmd_send(517);

    kao[0] = 1;
    kao[2] = 1;
    kao[1] = 1;
    kao2[0] = 1;
    kao2[2] = 1;
    kao2[1] = 1;
    imp[0] = 1;
    imp[2] = 1;
    imp[1] = 1;
    ans[0] = 0;
    ans[2] = 0;
    ans[1] = 0;
    serifu_no = 0;
    dec_bg_cache_step(17103);
    sub_sub_step_up();
}
