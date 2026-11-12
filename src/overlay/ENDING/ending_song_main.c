/* ending_song_main - ENDING overlay, Address: 0x8013B790, Size: 264 bytes (66 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[25];
} DispatchTable25;

extern DispatchTable25 ending_main7_tbl;
extern u8 now_sub_step;
extern u8 now_sub_sub_step;

extern void bustup_ani(void);
extern void k_scroll_set(void);
extern void k_disp_wait(void);
extern void update_anim_state(s32);
extern void message_window_show(void);
extern void basyo_show(void);
extern void hizuke_show(void);
extern void dir_scroll_main(void);

void ending_song_main(void)
{
    u8 state;
    u8 idx;
    DispatchTable25 tbl;

    tbl = ending_main7_tbl;

    state = now_sub_step;
    if (state == 7) {
        idx = now_sub_sub_step;
        if ((u32)idx < 16) {
            bustup_ani();
        }
    }

    idx = now_sub_sub_step;
    tbl.ptrs[idx](128);

    state = now_sub_step;
    if (state != 7) {
        return;
    }
    idx = now_sub_sub_step;
    if ((u32)idx >= 16) {
        return;
    }

    k_scroll_set();
    k_disp_wait();
    update_anim_state(2);
    message_window_show();
    basyo_show();
    hizuke_show();
    dir_scroll_main();
}
