/* @SymbolStart: 0x8014BFF4 */
/* Address: 0x8014BFFC */
/* Size: 380 bytes (95 insns) */
/* @SymbolSize: 384 */
/* @RodataBase: 0x8015B3C0 */
#include "types.h"

extern u8 now_sub_step;
extern void mes_window_switch(s32);
extern void bustup_ani(void);
extern void date_girl0_dispatch(void);
extern void date_scene_dispatch3(void);
extern void date_scene_dispatch2(void);
extern void date_girl3_dispatch(void);
extern void date_girl4_dispatch(void);
extern void date_mode_dispatch2(void);
extern void date_mode_dispatch(void);
extern void date_scene_dispatch(void);
extern void date_girl8_dispatch(void);
extern void date_girl9_dispatch(void);
extern void date_girl10_dispatch(void);
extern void date_girl11_dispatch(void);
extern void game_reset(void);
extern void k_scroll_set(void);
extern void k_disp_wait(void);
extern void update_anim_state(s32);
extern void message_window_show(void);
extern void basyo_show(void);
extern void hizuke_show(void);
extern void dir_scroll_main(void);
extern void don_move(void);

extern u8  now_sub_sub_step;
extern s32 event_girl_no;

void date_girl_main(void) {
    s32 idx;
    idx = now_sub_step;
    if (idx == 0 && now_sub_sub_step == 0) {
        mes_window_switch(1);
    }
    bustup_ani();
    switch (event_girl_no) {
    case 0:  date_girl0_dispatch(); break;
    case 1:  date_scene_dispatch3(); break;
    case 2:  date_scene_dispatch2(); break;
    case 3:  date_girl3_dispatch(); break;
    case 4:  date_girl4_dispatch(); break;
    case 5:  date_mode_dispatch2(); break;
    case 6:  date_mode_dispatch(); break;
    case 7:  date_scene_dispatch(); break;
    case 8:  date_girl8_dispatch(); break;
    case 9:  date_girl9_dispatch(); break;
    case 10: date_girl10_dispatch(); break;
    case 11: date_girl11_dispatch(); break;
    default: game_reset(); break;
    }
    k_scroll_set();
    k_disp_wait();
    update_anim_state(2);
    message_window_show();
    basyo_show();
    hizuke_show();
    dir_scroll_main();
    don_move();
}
