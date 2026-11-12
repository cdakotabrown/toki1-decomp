/* @PostProc: patch_stack_complete --old 144 --new 152 */
/* geko_sub_sub_dispatch27 - GEKO overlay, Address: 0x80140CF0, Size: 116 bytes (29 insns) */
/* @PostProc: remap_stack_slots --from 32 --to 40 */
/* @PostProc: patch_insn_reg_field 0x8:rt:v1:v0 0x18:rs:v1:v0 0x50:rs:v1:v0 0x54:rt:v0:t9 0x5c:rs:v0:t9 */
#include "types.h"

typedef struct {
    s32 w[3];
} Entry12;

typedef struct {
    Entry12 entries[9];
} Table108;

extern Table108 geko_sub_sub_dispatch27_tbl;
extern u8 now_sub_sub_step;

void geko_sub_sub_dispatch27(void) {
    u8 idx;
    Table108 buf;
    void (*fn)(void);

    buf = geko_sub_sub_dispatch27_tbl;
    idx = now_sub_sub_step;
    fn = ((void (**)(void))&buf)[idx];
    fn();
}
