/* @SymbolSize: 180 */
/* ending_bustup_name_main - ENDING overlay, Address: 0x80133550, Size: 180 bytes (45 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[43];
} JumpTable;

extern JumpTable ending_main1_tbl;
extern u8 now_sub_sub_step;

extern void bustup_ani(void);
extern void k_scroll_set(void);
extern void k_disp_wait(void);
extern void update_anim_state(s32);
extern void message_window_show(void);
extern void basyo_show(void);
extern void hizuke_show(void);
extern void dir_scroll_main(void);

void ending_bustup_name_main(void)
{
    u8 idx;
    JumpTable table;

    table = ending_main1_tbl;

    bustup_ani();

    idx = now_sub_sub_step;
    table.ptrs[idx](128);

    k_scroll_set();
    k_disp_wait();
    update_anim_state(2);
    message_window_show();
    basyo_show();
    hizuke_show();
    dir_scroll_main();
}
