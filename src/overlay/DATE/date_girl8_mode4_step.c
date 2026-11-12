/* @SymbolSize: 164 */
/* @PostProc: patch_stack_complete --old 200 --new 224 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* Address: 0x8014E290 */
/* Size: 164 bytes (41 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[44];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable date_main4_tbl;

extern void date_anim_disp(void);

void date_girl8_mode4_step(void)
{
    JumpTable table;

    table = date_main4_tbl;

    table.ptrs[now_sub_sub_step](128);

    if (now_sub_sub_step == 13) {
        date_anim_disp();
    }
}
