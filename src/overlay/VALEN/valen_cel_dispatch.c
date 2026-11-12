/* valen_cel_dispatch - VALEN overlay, Address: 0x80133E34, Size: 116 bytes (29 insns) */
/* @PostProc: patch_stack_complete --old 72 --new 96 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[12];
} FuncTable12;

extern FuncTable12 valen_main3_tbl;
extern u8 now_sub_sub_step;

void valen_cel_dispatch(void)
{
    FuncTable12 local;

    local = valen_main3_tbl;
    local.ptrs[now_sub_sub_step](128);
}
