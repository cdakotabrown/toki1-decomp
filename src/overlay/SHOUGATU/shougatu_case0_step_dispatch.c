/* @PostProc: patch_stack_complete --old 168 --new 176 */
/* shougatu_case0_step_dispatch - SHOUGATU overlay, Address: 0x8013D730, Size: 116 bytes (29 insns) */
/* @PostProc: remap_stack_slots --from 32 --to 40 */
/* @PostProc: patch_insn_reg_field 0x8:rt:v1:v0 0x18:rs:v1:v0 0x50:rs:v1:v0 0x54:rt:v0:t9 0x5c:rs:v0:t9 */
#include "types.h"

typedef struct {
    s32 w[3];
} Entry12;

typedef struct {
    Entry12 entries[11];
} Table132;

extern Table132 shougatu_main0_tbl;
extern u8 now_sub_sub_step;

void shougatu_case0_step_dispatch(void) {
    u8 idx;
    Table132 buf;
    void (*fn)(s32);

    buf = shougatu_main0_tbl;
    idx = now_sub_sub_step;
    fn = ((void (**)(s32))&buf)[idx];
    fn(128);
}
