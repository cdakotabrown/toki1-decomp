/* Address: 0x80134140 */
/* Size: 196 bytes (49 instructions) */
/* @SymbolSize: 196 */
/* @PostProc: patch_stack_complete --old 136 --new 152 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x68 */
/* @PostProc: remap_stack_slots --from 36 --to 48 */
/* @PostProc: patch_insn_reg_field 0x18:rt:a1:a2 0x28:rs:a1:a2 0x60:rt:v0:a1 0x6c:rs:v0:a1 0x6c:rt:v0:a1 0x74:rt:v0:a1 0x78:rd:v1:v0 0x78:rs:a1:a2 0x7c:rs:v1:v0 0x7c:rt:a0:v1 0x84:rt:a0:v1 0x90:rt:a0:v1 0x9c:rs:v1:v0 0xa0:rt:t8:a1 0xa4:rs:t8:a1 0xa4:rt:t8:a1 0xac:rd:t7:t8 0xac:rt:t8:a1 0xb0:rd:t9:t7 0xb0:rs:a1:a2 0xb0:rt:t7:t8 0xb4:rs:t9:t7 */
#include "types.h"

typedef struct {
    void (*f[25])(s32);
} FnTable25;

extern FnTable25 shougatu_main53e_tbl;
extern u8 shougatu_alone_flag;
extern volatile u8 now_sub_sub_step;

extern void fade_up_main(s32);
extern void fade_down_main(s32);
extern void sub_sub_step_up(s32);

void shougatu_main53e(void)
{
    FnTable25 tbl;
    s32 idx;

    tbl = shougatu_main53e_tbl;
    if (shougatu_alone_flag != 0) {
        idx = now_sub_sub_step;
        if (tbl.f[idx] == fade_up_main || tbl.f[idx] == fade_down_main) {
            tbl.f[idx] = sub_sub_step_up;
        }
    }
    tbl.f[now_sub_sub_step](128);
}
