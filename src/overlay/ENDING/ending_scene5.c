/* @SymbolSize: 380 */
/* ending_scene5 - ENDING overlay, Address: 0x80137360, Size: 380 bytes (95 insns; 388-byte audit window includes ending_exit_route's 2-word head) */
#include "types.h"

typedef void (*EndFn)(s32);

typedef struct {
    EndFn fn[6];
} EndFnTable;

extern void ending_line_scroll();
extern void k_disp_inc();
extern void grad_box_disp();
extern void dtd_on();
extern void ending_item_ani();
extern void ending_girl_dispatch();
extern void ending_flag_set();
extern void ending_kuchi_level_set();
extern void ending_serifu_run();

extern EndFnTable ending_scene5_tbl;
extern u8 now_sub_sub_step;
extern u8 now_sub_step;
extern u32 frame_counter;
extern s32 wink2_stop;

s32 ending_scene5(void)
{
    u8 step;
    u8 sub;
    EndFnTable buf;

    buf = ending_scene5_tbl;
    buf.fn[now_sub_sub_step](128);

    step = now_sub_sub_step;
    if ((u32)step >= 3 || (sub = now_sub_step, (u32)sub >= 6)) {
        return 0;
    }
    ending_line_scroll();
    k_disp_inc();

    if (buf.fn[now_sub_sub_step] == (EndFn)ending_serifu_run) {
        ending_kuchi_level_set();
    }
    grad_box_disp(-160, -120, 320, 240, 14, 0x00C0C0FF, 0x004080FF);
    dtd_on(14);
    ending_item_ani();
    ending_girl_dispatch();
    if (frame_counter % 240 == 0) {
        if (wink2_stop == 0) {
            ending_flag_set();
        }
    }
}
