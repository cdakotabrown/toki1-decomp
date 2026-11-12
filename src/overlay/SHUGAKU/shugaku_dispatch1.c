/* @PostProc: patch_stack_complete --old 96 --new 112 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
/* @PostProc: swap_registers_auto v1:v0 --bidirectional */
/* @PostProc: patch_insn_reg_field 0x6c:rt:v1:t9 0x74:rs:v1:t9 */
/* Address: 0x80138A00 */
/* Size: 124 bytes (31 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    s32 data[16];
} CopyStruct;

extern u8 now_sub_sub_step;
extern CopyStruct shugaku_main1_tbl;

void shugaku_dispatch1(void)
{
    CopyStruct local;
    u8 idx;
    FuncPtr fn;

    local = shugaku_main1_tbl;

    idx = now_sub_sub_step;
    fn = (FuncPtr)local.data[idx];
    fn(128);
}
