/* @SymbolSize: 144 */
/* @PostProc: patch_stack_complete --old 200 --new 216 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
/* Address: 0x8014DA78 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[42];
} JumpTable8014DA78;

extern JumpTable8014DA78 date_fn_dispatch_tbl;
extern u8 now_sub_sub_step;
extern s32 date_sprite_on;

extern void func_8014DB08(void);

void date_fn_dispatch(void)
{
    JumpTable8014DA78 table;
    s32 idx;

    table = date_fn_dispatch_tbl;
    idx = (s32)now_sub_sub_step;
    table.ptrs[idx](128);

    if (date_sprite_on != 0) {
        func_8014DB08();
    }
}
