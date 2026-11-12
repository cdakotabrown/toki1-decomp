/* taiiku_sub_dispatch0f - TAIIKU overlay, Address: 0x80138B10, Size: 116 bytes (29 insns) */
/* @PostProc: patch_stack_complete --old 72 --new 88 */
/* @PostProc: patch_insn_reg_field 0x8:rt:v1:v0 0x18:rs:v1:v0 0x50:rs:v1:v0 0x54:rt:v0:t9 0x5c:rs:v0:t9 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
#include "types.h"

typedef struct {
    s32 w[3];
} Entry12;

typedef struct {
    Entry12 entries[3];
} Table36;

extern Table36 taiiku_tama_step_tbl;
extern u8 now_sub_step;

void taiiku_sub_dispatch0f(void) {
    u8 idx;
    Table36 buf;
    void (*fn)(void);

    buf = taiiku_tama_step_tbl;
    idx = now_sub_step;
    fn = ((void (**)(void))&buf)[idx];
    fn();
}
