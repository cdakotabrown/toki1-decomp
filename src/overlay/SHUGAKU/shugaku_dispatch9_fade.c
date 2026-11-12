/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: remap_stack_slots --from 48 --to 44 */
/* @PostProc: swap_instructions --offset1 0xF4 --offset2 0xFC */
/* @PostProc: swap_instructions --offset1 0xF8 --offset2 0xFC */
/* @PostProc: swap_branch_operands --offsets 0xE0,0x130 */
/* @PostProc: patch_insn_reg_field 0xb0:rd:v1:t9 0xb4:rs:v1:t9 0xb4:rt:v1:t9 0xbc:rs:v1:t9 0xe4:rt:t9:t8 0xf0:rt:a0:a1 0xf4:rs:a0:a1 0x10c:rt:a0:a1 0x110:rs:a0:a1 0x110:rt:a0:a1 0x11c:rs:t9:t8 0x11c:rt:t9:t8 0x12c:rt:a0:a1 0x130:rs:t9:t8 0x134:rs:a0:a1 0x134:rt:v0:a0 0x13c:rt:v0:a0 0x140:rs:v0:a0 0x148:rt:t8:t0 0x14c:rs:t8:t0 0x14c:rt:t8:t0 0x154:rs:t8:t0 0x154:rt:t0:t7 0x158:rt:t0:t7 */
/* @SymbolSize: 348 */
#include "types.h"
#include "tokimeki/class_w.h"

extern volatile u8 now_sub_sub_step;
extern u8  cal_cw_bright;

typedef struct { s32 w[44]; } Tbl176;
extern Tbl176 shugaku_main9_tbl;
extern CLASS_W class_w[];

extern void date_light_effect(void);
extern void date_fade_in(void);
extern void date_fade_out(void);

void shugaku_dispatch9_fade(void)
{
    s32 sel;
    s32 tbl[44];
    s32 entry;
    s32 a1;
    s32 a0;
    void (*fn)(s32);

    sel = (s32)now_sub_sub_step;
    *(Tbl176 *)tbl = shugaku_main9_tbl;

    if ((u32)sel < 8U) {
        date_light_effect();
        sel = (s32)now_sub_sub_step;
    } else if ((u32)sel >= 29U && (u32)sel < 35U) {
        date_light_effect();
        sel = (s32)now_sub_sub_step;
    }

    fn = (void (*)(s32))tbl[sel];
    fn(128);

    sel = (s32)now_sub_sub_step;
    entry = tbl[sel];
    if (entry == (s32)date_fade_in) {
        a0 = (s32)cal_cw_bright;
        class_w[98].disp_flag = (u8)(class_w[98].disp_flag | 0x80);
        class_w[98].bright = (u8)a0;
    }

    a1 = (s32)cal_cw_bright;
    if ((u32)sel >= 13U && (u32)sel < 22U) {
        class_w[98].bright = (u8)a1;
    }
    if (entry == (s32)date_fade_out) {
        a0 = a1 & 0xFF;
        class_w[98].bright = (u8)a0;
        if ((u32)a0 < 8U) {
            class_w[98].disp_flag = (u8)(class_w[98].disp_flag & 0xFF7F);
        }
    }
}
