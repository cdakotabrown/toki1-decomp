/* shougatu_step_dispatch2 - SHOUGATU overlay, Address: 0x80137D68, Size: 124 bytes (31 insns) */
/* @PostProc: patch_insn_reg_field 0x8:rt:v1:v0 0x18:rs:v1:v0 0x58:rs:v1:v0 0x5c:rt:v0:t9 0x64:rs:v0:t9 */
#include "types.h"

typedef struct {
    s32 w[3];
} Entry12;

typedef struct {
    Entry12 entries[11];
    s32 tail;
} Table136;

extern Table136 shougatu_step_dispatch2_tbl;
extern u8 now_sub_sub_step;

void shougatu_step_dispatch2(void) {
    u8 idx;
    Table136 buf;
    void (*fn)(void);

    buf = shougatu_step_dispatch2_tbl;
    idx = now_sub_sub_step;
    fn = ((void (**)(void))&buf)[idx];
    fn();
}
