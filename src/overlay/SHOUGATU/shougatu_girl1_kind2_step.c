/* @PostProc: patch_stack_complete --old 136 --new 144 */
/* shougatu_girl1_kind2_step - SHOUGATU overlay, Address: 0x80139AA0, Size: 124 bytes (31 insns) */
/* @PostProc: remap_stack_slots --from 32 --to 40 */
/* @PostProc: patch_insn_reg_field 0x8:rt:v1:v0 0x18:rs:v1:v0 0x58:rs:v1:v0 0x5c:rt:v0:t9 0x64:rs:v0:t9 */
#include "types.h"

typedef struct {
    s32 w[3];
} Entry12;

typedef struct {
    Entry12 entries[8];
    s32 tail;
} Table100;

extern Table100 shougatu_subsub_jump5_tbl;
extern u8 now_sub_sub_step;

void shougatu_girl1_kind2_step(void) {
    u8 idx;
    Table100 buf;
    void (*fn)(void);

    buf = shougatu_subsub_jump5_tbl;
    idx = now_sub_sub_step;
    fn = ((void (**)(void))&buf)[idx];
    fn();
}
