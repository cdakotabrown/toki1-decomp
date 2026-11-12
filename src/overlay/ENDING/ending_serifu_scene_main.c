/* @SymbolSize: 176 */
/* ending_serifu_scene_main - ENDING overlay, Address: 0x80133AE0, Size: 176 bytes (44 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[9];
} DispatchTable9;

extern DispatchTable9 ending_main2_tbl;
extern u8 now_sub_sub_step;

extern void bustup_ani(void);
extern void k_scroll_set(void);
extern void k_disp_wait(void);
extern void update_anim_state(s32);
extern void message_window_show(void);
extern void basyo_show(void);
extern void hizuke_show(void);
extern void dir_scroll_main(void);

void ending_serifu_scene_main(void)
{
    u8 idx;
    DispatchTable9 tbl;

    tbl = ending_main2_tbl;

    bustup_ani();

    idx = now_sub_sub_step;
    tbl.ptrs[idx](128);

    k_scroll_set();
    k_disp_wait();
    update_anim_state(2);
    message_window_show();
    basyo_show();
    hizuke_show();
    dir_scroll_main();
}
