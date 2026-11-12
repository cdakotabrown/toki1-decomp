/* @SymbolSize: 116 */
/* @PostProc: patch_stack_complete --old 104 --new 120 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
/* @PostProc: swap_registers v1:v0 */
/* @PostProc: patch_insn_reg_field 0x64:rt:v0:t9 0x6c:rs:v0:t9 */
/* Address: 0x80139C30 */
/* Size: 116 bytes (29 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    s32 data[18];
} CopyStruct;

extern u8 now_sub_sub_step;
extern CopyStruct shugaku_main3_tbl;

void shugaku_dispatch3(void)
{
    CopyStruct local;
    u8 idx;
    FuncPtr fn;

    local = shugaku_main3_tbl;

    idx = now_sub_sub_step;
    fn = (FuncPtr)local.data[idx];
    fn(128);
}
