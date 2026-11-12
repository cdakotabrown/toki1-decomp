/* Address: 0x8014AC34 */
/* Size: 224 bytes (56 instructions) */
/* @SymbolSize: 224 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
#include "types.h"

extern void draw2d3d(s32, s32);
extern void class_switch(s32);
extern void class_clear_f(void);
extern void back_clear_switch(s32);
extern void tpage_buf_clear(void);
extern void prim_init(void);
extern void cursor_init(s32);
extern void hizuke_init(void);
extern void icon_disp_switch(s32);
extern void parameter_disp_switch(s32);
extern void message_window_init(void);
extern void text_sys_init(void);
extern void mes_window_switch(s32);
extern void set_dec_bri(s32);
extern s32  value_cache_find_or_load(s32, s32);
extern void sub_sub_step_up(void);
extern void indexed_byte_lsb_set(s32, s32);

void etc_init0(void)
{
    s32 v0;

    draw2d3d(1, 0);
    class_switch(1);
    class_clear_f();
    back_clear_switch(1);
    tpage_buf_clear();
    prim_init();
    cursor_init(0);
    hizuke_init();
    icon_disp_switch(0);
    parameter_disp_switch(0);
    message_window_init();
    text_sys_init();
    mes_window_switch(0);
    set_dec_bri(0);
    v0 = value_cache_find_or_load(16469, 1);
    if (v0 != -1) {
        if (v0 == 0) {
            goto block;
        }
        if (v0 != 1) {
            return;
        }
    block:
        sub_sub_step_up();
        sub_sub_step_up();
        indexed_byte_lsb_set(1, v0);
        return;
    }
    sub_sub_step_up();
}
