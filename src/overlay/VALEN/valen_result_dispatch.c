/* @PostProc: patch_stack_complete --old 64 --new 88 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* valen_result_dispatch - VALEN overlay, Address: 0x80133920, Size: 124 bytes (31 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[10];
} FuncTable10;

extern FuncTable10 valen_main2_tbl;
extern u8 now_sub_sub_step;

void valen_result_dispatch(void)
{
    FuncTable10 local;

    local = valen_main2_tbl;
    local.ptrs[now_sub_sub_step](128);
}
