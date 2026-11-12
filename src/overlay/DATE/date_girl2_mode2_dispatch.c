/* @SymbolSize: 172 */
/* @PostProc: patch_stack_complete --old 184 --new 208 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x80150BC0 */
/* Size: 172 bytes (43 instructions) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[40];
} JumpTable;

extern u8 now_sub_sub_step;
extern s32 date_sprite_on;
extern u8 update_anim_state_tbl4;
extern JumpTable date_girl2_mode2_dispatch_tbl;

extern void func_80150C6C(void);

void date_girl2_mode2_dispatch(void) {
    JumpTable table;

    table = date_girl2_mode2_dispatch_tbl;
    table.ptrs[now_sub_sub_step]();

    if (date_sprite_on != 0) {
        func_80150C6C();
        update_anim_state_tbl4 = 9;
    } else {
        update_anim_state_tbl4 = 15;
    }
}
