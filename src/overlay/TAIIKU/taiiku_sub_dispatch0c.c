/* @PostProc: patch_stack_complete --old 64 --new 80 */
/* @PostProc: swap_registers v1:v0 */
/* @PostProc: patch_insn_reg_field 144:rt:v0:t9 152:rs:v0:t9 */
/* @PostProc: remap_stack_slots --from 32 --to 44 */
/* Address: 0x80145494 */
/* Size: 160 bytes (40 insns) */
#include "types.h"

typedef struct { u32 e[8]; } Tbl8;
extern Tbl8 taiiku_spoon_step_tbl;
extern u8 now_sub_step;

void taiiku_sub_dispatch0c(void) {
    Tbl8 buf;
    u32 idx;
    void (*fn)(void);

    buf = taiiku_spoon_step_tbl;
    idx = now_sub_step;
    fn = (void (*)(void))buf.e[idx];
    fn();
}
