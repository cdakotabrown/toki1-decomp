/* @SymbolStart: 0x80149240 */
/* @SymbolSize: 212 */
/* @RodataBase: 0x8014D8AC */
#include "types.h"

extern s32 get_last_gamen_mode(void);
extern s32 dec_bg_show(s32 a0);
extern void hizuke_show(void);
extern void message_window_show(void);
extern void basyo_show(void);
extern void update_anim_state(s32 a0);
extern void mascot_move(void);
extern void step_directional_scroll_once(void);
extern void etc_cw32_show(void);
extern void etc_step0(void);
extern void etc_siken_main1(void);
extern void etc_siken_main2(void);
extern void etc_siken_main3(void);
extern void etc_flag_show(void);

extern u8 now_sub_step;

void etc_siken_scene_main(void) {
    s32 cmd = now_sub_step;

    s32 v0;
    switch (cmd) {
    case 0:
        etc_step0();
        break;
    case 1:
        etc_siken_main1();
        break;
    case 2:
        etc_siken_main2();
        break;
    case 3:
        etc_siken_main3();
        break;
    case 4:
        etc_flag_show();
        break;
    }
    step_directional_scroll_once();
    hizuke_show();
    message_window_show();
    basyo_show();
    update_anim_state(0);
    v0 = get_last_gamen_mode();
    dec_bg_show(v0);
    etc_cw32_show();
    mascot_move();
}
