/* @PostProc: patch_stack_complete --old 248 --new 272 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* valen_list_dispatch - VALEN overlay, Address: 0x80132760, Size: 136 bytes (34 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[56];
} FuncTable56;

extern FuncTable56 valen_main1_tbl;
extern u8 now_sub_sub_step;

void valen_list_dispatch(void)
{
    FuncTable56 local;

    local = valen_main1_tbl;
    local.ptrs[now_sub_sub_step](128);
}
