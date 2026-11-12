/* shougatu_girl3_kind0_step - SHOUGATU overlay, Address: 0x80138BB0, Size: 124 bytes (31 insns) */
/* @PostProc: patch_insn_reg_field 0x8:rt:v1:v0 0x18:rs:v1:v0 0x58:rs:v1:v0 0x5c:rt:v0:t9 0x64:rs:v0:t9 */
#include "types.h"

typedef struct {
    s32 w[3];
} Entry12;

typedef struct {
    Entry12 entries[7];
    s32 tail;
} Table88;

extern Table88 shougatu_subsub_jump0_tbl;
extern u8 now_sub_sub_step;

void shougatu_girl3_kind0_step(void) {
    u8 idx;
    Table88 buf;
    void (*fn)(void);

    buf = shougatu_subsub_jump0_tbl;
    idx = now_sub_sub_step;
    fn = ((void (**)(void))&buf)[idx];
    fn();
}
