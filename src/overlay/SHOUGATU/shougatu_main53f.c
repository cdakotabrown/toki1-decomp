/* Address: 0x80134930 */
/* Size: 292 bytes (73 instructions) */
/* @PostProc: patch_stack_complete --old 176 --new 192 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x68 */
/* @PostProc: remap_stack_slots --from 52 --to 64 */
/* @PostProc: patch_insn_reg_field 0x18:rt:a1:v1 0x28:rs:a1:v1 0x60:rt:v0:a1 0x6c:rs:v0:a1 0x6c:rt:v0:a1 0x74:rt:v0:a1 0x78:rd:v1:v0 0x78:rs:a1:v1 0x7c:rs:v1:v0 0x90:rs:v1:v0 0x94:rt:t8:a1 0x98:rs:t8:a1 0x98:rt:t8:a1 0xa0:rd:t7:t8 0xa0:rt:t8:a1 0xa4:rd:t9:t7 0xa4:rs:a1:v1 0xa4:rt:t7:t8 0xa8:rs:t9:t7 */
#include "types.h"

typedef struct {
    void (*f[31])(s32);
} FnTable30;

extern FnTable30 shougatu_main53f_tbl;
extern u8 shougatu_alone_flag;
extern u8 shougatu_omikuji_box_on;
extern volatile u8 now_sub_sub_step;

extern void fade_up_main(s32);
extern void sub_sub_step_up(s32);
extern void grad_box_disp(s32, s32, s32, s32, s32, s32, s32);
extern void dtd_on(s32);

void shougatu_main53f(void)
{
    FnTable30 tbl;
    s32 idx;

    tbl = shougatu_main53f_tbl;
    if (shougatu_alone_flag != 0) {
        idx = now_sub_sub_step;
        if (tbl.f[idx] == fade_up_main) {
            tbl.f[idx] = sub_sub_step_up;
        }
    }
    tbl.f[now_sub_sub_step](64);
    if (shougatu_omikuji_box_on != 0) {
        grad_box_disp(-160, -120, 320, 100, 13, 0xE0FF, 0xFFFF);
        grad_box_disp(-160, -20, 320, 60, 13, 24672, 0xE0FF);
        dtd_on(13);
    }
}
