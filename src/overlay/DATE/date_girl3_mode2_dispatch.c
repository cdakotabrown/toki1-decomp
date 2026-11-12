/* @SymbolSize: 160 */
/* @PostProc: patch_stack_complete --old 256 --new 264 */
/* @PostProc: remap_stack_slots --from 36 --to 40 */
/* Address: 0x80151F84 */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[55];
} JumpTable80151F84;

extern JumpTable80151F84 date_girl3_mode2_dispatch_tbl;
extern u8 now_sub_sub_step;

extern void date_bg_scroll_disp(void);

void date_girl3_mode2_dispatch(void)
{
    JumpTable80151F84 table;
    s32 idx;

    table = date_girl3_mode2_dispatch_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx]();

    idx = (s32)now_sub_sub_step;
    if (idx == 26 || idx == 27) {
        date_bg_scroll_disp();
    }
}
