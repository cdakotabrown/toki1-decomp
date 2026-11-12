/* @PostProc: patch_stack_complete --old 200 --new 208 */
/* @PostProc: remap_stack_slots --from 36 --to 40 */
/* @PostProc: patch_insn_reg_field 0x18:rt:v1:v0 */
/* @PostProc: patch_insn_reg_field 0x28:rs:v1:v0 */
/* @PostProc: patch_insn_reg_field 0x74:rs:v1:v0 */
/* @PostProc: patch_insn_reg_field 0x78:rt:v0:t9 */
/* @PostProc: patch_insn_reg_field 0x80:rs:v0:t9 */
/* Address: 0x80137890 */
/* Size: 136 bytes (34 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    s32 data[41];
} CopyStruct;

extern u8 now_sub_sub_step;
extern CopyStruct shugaku_dispatch0_tbl;

void shugaku_dispatch0(void)
{
    CopyStruct local;
    u8 idx;
    FuncPtr fn;

    local = shugaku_dispatch0_tbl;

    idx = now_sub_sub_step;
    fn = (FuncPtr)local.data[idx];
    fn(128);
}
