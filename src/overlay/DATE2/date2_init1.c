/* @SymbolSize: 116 */
/* date2_init1 -- DATE2 overlay, Address: 0x80132BEC, Size: 140 bytes (35 insns) */
#include "types.h"

void draw2d3d(s32 arg0, s32 arg1);
void class_switch(s32 arg0);
void prim_init(void);
void class_clear_all(s32 arg0);
void back_clear_switch(s32 arg0);
void tpage_buf_clear(void);
void shared_system_state_reset(void);
void message_window_init(void);
void hizuke_init(void);
void yazirushi_off(void);
void sub_sub_step_up(void);

void date2_init1(void)
{
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    shared_system_state_reset();
    message_window_init();
    hizuke_init();
    yazirushi_off();
    sub_sub_step_up();
}
