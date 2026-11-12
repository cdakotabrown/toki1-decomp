/* @SymbolStart: 0x80134ECC */
/* @SymbolSize: 252 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0xa8 --offset2 0xac */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* taco_init0 - TACO overlay, Address: 0x80134EE8 (cross-prologue head at */
#include "types.h"

extern s32 taco_view_flag;
extern u8 back_clear_g;
extern u8 back_clear_r;

extern s8 taco_menu_csr;
extern s8 back_clear_b;
extern u8 cursor_work_bak[];
extern s32 D_800EE840[];
extern s32 basyo_flag0[];
extern s32 screen_w[];
extern s32 girl1_flag[];

extern void all_object_disp_off(void);
extern void draw2d3d(s32, s32);
extern void class_switch(s32);
extern void k_reset(s32);
extern s32 image_rect_store(s32, s32, s32, s32, u8 *);
extern void palette_upload_enqueue(u8 *, s32, s32, s32, s32);
extern void taco_obj_init(void);
extern void taco_model_init(void);
extern void sub_sub_step_up(void);

void taco_init0(void)
{
    s32 *p;

    taco_view_flag = 0;
    back_clear_g = 0;
    back_clear_r = 0;
    taco_menu_csr = 0;
    back_clear_b = 0;
    all_object_disp_off();
    draw2d3d(1, 1);
    class_switch(1);
    k_reset(0);
    image_rect_store(0, 495, 256, 1, cursor_work_bak);

    p = D_800EE840;
    do {
        p++;
        *(s32 *)((char *)p + 5688) = 0;
    } while ((u32)p < (u32)basyo_flag0);

    p = screen_w;
    do {
        p += 4;
        *(s32 *)((char *)p + 6192) = 0;
        *(s32 *)((char *)p + 6196) = 0;
        *(s32 *)((char *)p + 6200) = 0;
        *(s32 *)((char *)p + 6188) = 0;
    } while ((u32)p != (u32)girl1_flag);

    palette_upload_enqueue(cursor_work_bak, 31, 1, 1, 0);
    taco_obj_init();
    taco_model_init();
    sub_sub_step_up();
}
