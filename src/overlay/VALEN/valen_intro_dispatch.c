/* @PostProc: patch_stack_complete --old 152 --new 168 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* valen_intro_dispatch - VALEN overlay, Address: 0x801323D0, Size: 124 bytes (31 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[31];
} FuncTable;

extern FuncTable valen_main0_tbl;
extern u8 now_sub_sub_step;

void valen_intro_dispatch(void)
{
    FuncTable local;

    local = valen_main0_tbl;
    local.ptrs[now_sub_sub_step](128);
}
