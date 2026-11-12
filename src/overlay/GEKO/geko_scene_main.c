/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all IDO52_UGEN_TEMP_REG=10:t6 */
/* @RodataBase: 0x80144840 */
/* Address: 0x801387E4 */
#include "types.h"

extern s32 event_girl_no;
extern s32 dec_bg_on;
extern s32 script_on;
extern s32 date_sprite_on;
extern s32 pad_data;
extern s32 menu_on;
extern volatile u8 cursor_cw_disp;

extern void bustup_ani(void);
extern void game_reset(void);
extern void k_scroll_set(void);
extern void k_disp_wait(void);
extern void update_anim_state(s32);
extern void message_window_show(void);
extern void basyo_show(void);
extern void hizuke_show(void);
extern void dir_scroll_main(void);
extern void don_move(void);
extern void dec_bg_show(s32);
extern void parameter_show(void);
extern void step_directional_scroll_once(void);

extern void geko_girl0_main(void);
extern void geko_girl1_main(void);
extern void geko_girl2_main(void);
extern void geko_girl3_main(void);
extern void geko_girl4_main(void);
extern void geko_girl5_main(void);
extern void geko_girl6_main(void);
extern void geko_girl7_main(void);
extern void geko_girl8_main(void);
extern void geko_girl9_main(void);
extern void geko_main0a(void);
extern void geko_main0b(void);
extern void geko_main0c(void);
extern void geko_main0d(void);

void geko_scene_main(void)
{
    u8 v;

    bustup_ani();
    switch ((u32)event_girl_no) {
    case 0:  geko_girl0_main(); break;
    case 1:  geko_girl1_main(); break;
    case 2:  geko_girl2_main(); break;
    case 3:  geko_girl3_main(); break;
    case 4:  geko_girl4_main(); break;
    case 5:  geko_girl5_main(); break;
    case 6:  geko_girl6_main(); break;
    case 7:  geko_girl7_main(); break;
    case 8:  geko_girl8_main(); break;
    case 9:  geko_girl9_main(); break;
    case 10: geko_main0a(); break;
    case 11: geko_main0b(); break;
    case 12: geko_main0c(); break;
    case 13: geko_main0d(); break;
    default: game_reset(); break;
    }

    k_scroll_set();
    k_disp_wait();
    if (dec_bg_on == 0) {
        update_anim_state(2);
    }
    message_window_show();
    basyo_show();
    hizuke_show();
    if (dec_bg_on == 0) {
        dir_scroll_main();
    }
    don_move();
    if (dec_bg_on == 0) {
        return;
    }

    update_anim_state(1);
    dec_bg_show(0);
    parameter_show();

    if (script_on != 0) {
        goto skip;
    }
    if (date_sprite_on != 0) {
        goto skip;
    }
    if ((pad_data & 0xF000F000) != 0) {
        goto setbit;
    }
    if (menu_on != 1) {
        goto skip;
    }
setbit:
    v = cursor_cw_disp;
    v |= 0x80;
    cursor_cw_disp = v;
skip:
    v = cursor_cw_disp;
    if (date_sprite_on != 0) {
        v &= 0x7F;
    }
    cursor_cw_disp = (u8)v;
    if ((v & 0x80) != 0) {
        step_directional_scroll_once();
    }
}
