/* @SymbolSize: 116 */
/* master_sub_step_dispatch2 - MASTER overlay, Address: 0x80133A2C, Size: 124 bytes (31 insns) */
/* @PostProc: patch_insn_reg_field 0x18:rt:v1:v0 0x28:rs:v1:v0 0x60:rs:v1:v0 0x64:rt:v0:t9 0x6c:rs:v0:t9 */
#include "types.h"

typedef struct {
    s32 w[3];
} Entry12;

typedef struct {
    Entry12 entries[10];
} Table120;

extern Table120 master_sub_step_dispatch2_tbl;
extern u8 now_sub_step;

void master_sub_step_dispatch2(void) {
    u8 idx;
    Table120 buf;
    void (*fn)(void);

    buf = master_sub_step_dispatch2_tbl;
    idx = now_sub_step;
    fn = ((void (**)(void))&buf)[idx];
    fn();
}
