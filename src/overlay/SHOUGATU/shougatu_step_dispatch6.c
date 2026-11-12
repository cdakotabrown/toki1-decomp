/* shougatu_step_dispatch6 - SHOUGATU overlay, Address: 0x801384C4, Size: 116 bytes (29 insns) */
/* @PostProc: patch_insn_reg_field 0x8:rt:v1:v0 0x18:rs:v1:v0 0x50:rs:v1:v0 0x54:rt:v0:t9 0x5c:rs:v0:t9 */
#include "types.h"

typedef struct {
    s32 w[3];
} Entry12;

typedef struct {
    Entry12 entries[8];
} Table96;

extern Table96 shougatu_step_dispatch6_tbl;
extern u8 now_sub_sub_step;

void shougatu_step_dispatch6(void) {
    u8 idx;
    Table96 buf;
    void (*fn)(void);

    buf = shougatu_step_dispatch6_tbl;
    idx = now_sub_sub_step;
    fn = ((void (**)(void))&buf)[idx];
    fn();
}
