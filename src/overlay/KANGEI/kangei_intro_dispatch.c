/* @PostProc: patch_stack_complete --old 88 --new 104 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @SymbolSize: 116 */
/* kangei_intro_dispatch -- KANGEI overlay, Address: 0x80133C20, Size: 116 bytes (29 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[15];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable kangei_main2_tbl;

void kangei_intro_dispatch(void)
{
    JumpTable table;

    table = kangei_main2_tbl;

    table.ptrs[now_sub_sub_step](128);
}
