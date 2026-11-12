/* @PostProc: patch_stack_complete --old 200 --new 216 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x801336A4 */
/* Size: 124 bytes (31 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    s32 data[43];
} CopyStruct;

extern u8 now_sub_sub_step;
extern CopyStruct shugaku_main7_tbl;

void shugaku_dispatch7(void)
{
    CopyStruct local;

    local = shugaku_main7_tbl;

    ((FuncPtr)local.data[now_sub_sub_step])(128);
}
