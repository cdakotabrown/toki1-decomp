/* @SymbolSize: 284 */
/* @PostProc: patch_insn_reg_field 0x14:rt:v0:v1 0x18:rs:v0:v1 0x18:rt:v0:v1 0x28:rs:v0:v1 0x34:rs:v0:v1 0x3c:rs:v0:v1 */
/* Address: 0x801465E4 */
/* Size: 284 bytes (71 instructions) */
#include "types.h"

extern void debug_hex_show(s32, s32, s32);
extern void dec_bg_show(s32);
extern void parameter_show(void);
extern void hizuke_show(void);
extern void message_window_show(void);
extern void icon_cursor_move(s32);
extern void update_anim_state(s32);
extern void mascot_disp_switch(s32);
extern void mascot_move(void);
extern void step_directional_scroll_once(void);
extern void etc_week_main(void);
extern void etc_sub_sub_main0(void);
extern void etc_parameter_main(void);
extern u8 now_youbi;
extern u8 now_sub_step;
extern s32 work_fuku[];

void etc_exam_week_main(void)
{
    s32 s0;
    s32 *s1;

    switch (now_sub_step) {
    case 0:
        etc_sub_sub_main0();
        goto common;
    case 1:
        etc_week_main();
        goto common;
    case 2:
        etc_parameter_main();
        goto common;
    }
common:
    update_anim_state(1);
    message_window_show();
    hizuke_show();
    parameter_show();
    step_directional_scroll_once();
    icon_cursor_move(1);
    if (now_youbi == 5) {
        mascot_disp_switch(0);
    } else {
        mascot_move();
    }
    dec_bg_show(0);
    s1 = work_fuku;
    s0 = 40;
    do {
        debug_hex_show(s1[10], 64, s0);
        debug_hex_show(s1[4], 80, s0);
        s0 += 8;
        s1 += 1;
    } while (s0 != 88);
}
