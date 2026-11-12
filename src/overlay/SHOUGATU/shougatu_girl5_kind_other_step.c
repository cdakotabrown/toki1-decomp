/* @PostProc: patch_stack_complete --old 112 --new 120 */
/* shougatu_girl5_kind_other_step - SHOUGATU overlay, Address: 0x8013A37C, Size: 124 bytes (31 insns) */
/* @PostProc: remap_stack_slots --from 32 --to 40 */
/* @PostProc: patch_insn_reg_field 0x8:rt:v1:v0 0x18:rs:v1:v0 0x58:rs:v1:v0 0x5c:rt:v0:t9 0x64:rs:v0:t9 */
#include "types.h"

typedef struct {
    s32 w[3];
} Entry12;

typedef struct {
    Entry12 entries[6];
    s32 tail;
} Table76;

extern Table76 shougatu_girl5_other_substep_tbl;
extern u8 now_sub_sub_step;

void shougatu_girl5_kind_other_step(void) {
    u8 idx;
    Table76 buf;
    void (*fn)(void);

    buf = shougatu_girl5_other_substep_tbl;
    idx = now_sub_sub_step;
    fn = ((void (**)(void))&buf)[idx];
    fn();
}
