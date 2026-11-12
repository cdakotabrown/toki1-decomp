/* shougatu_dispatch9 - SHOUGATU overlay, Address: 0x80141748, Size: 116 bytes (29 insns) */
/* @PostProc: patch_insn_reg_field 0x8:rt:v1:v0 0x18:rs:v1:v0 0x50:rs:v1:v0 0x54:rt:v0:t9 0x5c:rs:v0:t9 */
#include "types.h"

typedef struct {
    s32 w[3];
} Entry12;

typedef struct {
    Entry12 entries[4];
} Table48;

extern Table48 shougatu_main59_tbl;
extern u8 now_sub_sub_step;

void shougatu_dispatch9(void) {
    u8 idx;
    Table48 buf;
    void (*fn)(s32);

    buf = shougatu_main59_tbl;
    idx = now_sub_sub_step;
    fn = ((void (**)(s32))&buf)[idx];
    fn(128);
}
