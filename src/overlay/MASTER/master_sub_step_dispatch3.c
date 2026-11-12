/* master_sub_step_dispatch3 - MASTER overlay, Address: 0x80139150, Size: 148 bytes (37 insns) */
/* @PostProc: patch_stack_complete --old 64 --new 72 */
/* @PostProc: remap_stack_slots --from 32 --to 40 */
/* @PostProc: patch_insn_reg_field 0x24:rt:v1:v0 0x28:rs:v1:v0 0x34:rs:v1:v0 0x40:rs:v1:v0 */
/* @PostProc: patch_insn_reg_field 0x4c:rs:v1:v0 0x58:rs:v1:v0 0x64:rs:v1:v0 0x70:rs:v1:v0 */
/* @PostProc: patch_insn_reg_field 0x80:rs:v1:v0 0x84:rt:v0:t9 0x8c:rs:v0:t9 */
#include "types.h"

typedef struct {
    s32 entries[7];
} Table7;

extern Table7 master_sub_step_dispatch3_tbl;
extern u8 now_sub_step;

void master_sub_step_dispatch3(void) {
    u8 idx;
    Table7 buf;
    void (*fn)(void);

    buf = master_sub_step_dispatch3_tbl;
    idx = now_sub_step;
    fn = ((void (**)(void))&buf)[idx];
    fn();
}
