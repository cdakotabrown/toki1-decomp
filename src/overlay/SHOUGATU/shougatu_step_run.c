/* Address: 0x80136058 */
/* Size: 208 bytes (52 instructions) */
/* @SymbolSize: 208 */
/* @PostProc: patch_stack_complete --old 240 --new 256 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x74 */
/* @PostProc: remap_stack_slots --from 40 --to 52 */
/* @PostProc: patch_insn_reg_field 0x18:rt:a1:a2 0x28:rs:a1:a2 0x60:rt:v0:a1 0x78:rs:v0:a1 0x78:rt:v0:a1 0x80:rt:v0:a1 0x84:rd:v1:v0 0x84:rs:a1:a2 0x88:rs:v1:v0 0x88:rt:a0:v1 0x90:rt:a0:v1 0x9c:rt:a0:v1 0xa8:rs:v1:v0 0xac:rt:t8:a1 0xb0:rs:t8:a1 0xb0:rt:t8:a1 0xb8:rd:t7:t8 0xb8:rt:t8:a1 0xbc:rd:t9:t7 0xbc:rs:a1:a2 0xbc:rt:t7:t8 0xc0:rs:t9:t7 */
#include "types.h"

typedef struct {
    void (*f[50])(s32);
} FnTable50;

extern FnTable50 shougatu_step_run_tbl;
extern u8 shougatu_alone_flag;
extern volatile u8 now_sub_sub_step;

extern void fade_up_main(s32);
extern void fade_down_main(s32);
extern void sub_sub_step_up(s32);

void shougatu_step_run(void)
{
    FnTable50 tbl;
    s32 idx;

    tbl = shougatu_step_run_tbl;
    if (shougatu_alone_flag != 0) {
        idx = now_sub_sub_step;
        if (tbl.f[idx] == fade_up_main || tbl.f[idx] == fade_down_main) {
            tbl.f[idx] = sub_sub_step_up;
        }
    }
    tbl.f[now_sub_sub_step](128);
}
