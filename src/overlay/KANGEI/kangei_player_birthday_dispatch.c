/* @PostProc: patch_stack_complete --old 184 --new 208 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* @SymbolSize: 124 */
/* kangei_player_birthday_dispatch -- KANGEI overlay, Address: 0x801383D0, Size: 124 bytes (31 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[40];
} JumpTable;

extern u8 now_sub_sub_step;
extern JumpTable kangei_birthday_substep_tbl;

void kangei_player_birthday_dispatch(void)
{
    JumpTable table;

    table = kangei_birthday_substep_tbl;

    table.ptrs[now_sub_sub_step](128);
}
