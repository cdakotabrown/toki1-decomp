/* @PostProc: patch_stack_complete --old 144 --new 152 */
/* shougatu_girl6_kind0_step - SHOUGATU overlay, Address: 0x8013A63C, Size: 116 bytes (29 insns) */
/* @PostProc: remap_stack_slots --from 32 --to 40 */
/* @PostProc: patch_insn_reg_field 0x8:rt:v1:v0 0x18:rs:v1:v0 0x50:rs:v1:v0 0x54:rt:v0:t9 0x5c:rs:v0:t9 */
#include "types.h"

typedef struct {
    s32 w[3];
} Entry12;

typedef struct {
    Entry12 entries[9];
} Table108;

extern Table108 shougatu_girl6_kind0_substep_tbl;
extern u8 now_sub_sub_step;

void shougatu_girl6_kind0_step(void) {
    u8 idx;
    Table108 buf;
    void (*fn)(s32);

    buf = shougatu_girl6_kind0_substep_tbl;
    idx = now_sub_sub_step;
    fn = ((void (**)(s32))&buf)[idx];
    fn(128);
}
