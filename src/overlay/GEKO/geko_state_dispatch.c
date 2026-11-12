/* @PostProc: swap_branch_operands --offsets 0x98,0xb8 */
/* @PostProc: patch_insn_reg_field 0x7c:rt:t4:v1 0x80:rs:t4:v1 0x80:rt:t4:v1 0x84:rt:t8:t5 0x88:rd:t5:t4 0x88:rt:t4:v1 0x8c:rt:t5:t4 0x94:rs:t8:t5 0x94:rt:t8:t5 0x98:rs:t8:t5 0x9c:rt:t0:t6 0xa0:rt:t7:t8 0xa4:rs:t7:t8 0xa4:rt:t7:t8 0xac:rs:t7:t8 0xac:rt:t6:t7 0xb0:rt:t6:t7 0xb4:rs:t0:t6 0xb4:rt:t0:t6 0xb8:rs:t0:t6 0xbc:rt:t1:t0 0xc0:rs:t1:t0 0xc0:rt:t1:t0 0xc8:rs:t1:t0 0xc8:rt:t2:t1 0xcc:rt:t2:t1 */
/* geko_state_dispatch -- GEKO overlay, Address: 0x8013A29C, Size: 216 bytes (54 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

typedef void (*FuncPtr)(s32 a0);

typedef struct {
    FuncPtr ptrs[55];
} JumpTable;

extern JumpTable geko_state_dispatch_tbl;

extern void date_fade_in(s32 a0);
extern void date_fade_out(s32 a0);
extern void geko_cw_bright_set(void);

extern u8 now_sub_sub_step;
extern CLASS_W class_w[];

void geko_state_dispatch(void)
{
    s32 pad_hi;
    JumpTable table;
    FuncPtr fn;

    table = geko_state_dispatch_tbl;
    table.ptrs[now_sub_sub_step](128);

    fn = table.ptrs[now_sub_sub_step];
    if (fn == date_fade_in) {
        class_w[98].disp_flag |= 0x80;
    }
    if (fn == date_fade_out) {
        class_w[98].disp_flag &= ~0x80;
    }
    geko_cw_bright_set();
}
