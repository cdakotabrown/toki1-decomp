/* Function: xa_wait */
/* Address: 0x800521DC */
/* Size: 580 bytes (145 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=a0:24 */
/* @CompileEnv: IDO52_AS1_TIE_SRC_ORDER=bb:8 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_stack_complete --old 32 --new 56 */
/* @PostProc: remap_stack_slots 24:44,28:40 */
/* @PostProc: patch_insn_reg_field 0x50:rt:a0:a1 0x54:rt:a0:a1 0x54:rs:a0:a1 0x5C:rs:a0:a1 */
/* @PostProc: swap_branch_operands --offsets 0x60 */
/* @PostProc: swap_instructions --offset1 0x180 --offset2 0x184 */
/* `@CompileFlags: -Wc,-pic1` + `--old 48 --new 56` also reports 145/145, and */
#include "types.h"

extern s32 get_now_k(void);
extern s32 k_disp_inc(void);
extern void k_disp_switch(s32, s32);
extern void *get_k_work(s32);
extern void mes_arrow_anim(void);
extern void yazirushi_disp_switch(s32);
extern void k_disp_goto_line_end(void);

extern u8 k_mode;
extern u8 k_no;
extern u8 k_cnt;
extern u8 yazirushi_cw_disp;
extern s32 pad_trig;
extern u8 step_mode;

void xa_wait(void)
{
    void *sp28;
    s32 sp2C;
    s32 v0;
    s32 v1;
    s32 count;

    sp2C = 0;
    v0 = get_now_k();
    v1 = k_mode;

    if (!(v1 & 0x80)) goto outer_else;

    if (!((u32)(k_no + 2) < (u32)v0)) {

        sp2C = k_disp_inc();
        v0 = get_now_k();
        if (k_no + 3 == v0) {
            if ((u32)k_cnt < (u32)(k_no + 3)) goto label_2384;
            k_disp_switch(v0, 0);
        }
        goto label_2388;
    }

    count = v1 & 0x7F;
    if (count != 0) goto label_count_nonzero;

    k_disp_switch(k_no, 0);
    sp28 = get_k_work(k_no + 1);
    v0 = (s32)get_k_work(k_no + 2);
    *((s16 *)sp28 + 1) -= 2;
    *((s16 *)v0 + 1) -= 2;
    k_mode++;
    goto label_2384;

label_count_nonzero:

    if (count == 0) goto label_2388;
    sp28 = get_k_work(k_no + 1);
    v0 = (s32)get_k_work(k_no + 2);
    *((s16 *)sp28 + 1) -= 2;
    *((s16 *)v0 + 1) -= 2;
    if (*((s16 *)sp28 + 1) < 51) {
        *((s16 *)sp28 + 1) = 50;
        *((s16 *)v0 + 1) = 66;
        k_mode--;
        k_no++;
        k_disp_switch((u8)(k_no) + 2, 1);
        goto label_2388;
    }
    goto label_2384;

outer_else:
    sp2C = k_disp_inc();
    get_now_k();

label_2384:
    (void)sp28;
    (void)sp2C;

label_2388:
    if (sp2C == 1) {
        if (yazirushi_cw_disp & 0x80) {
            mes_arrow_anim();
        } else {
            yazirushi_disp_switch(1);
        }
    }

    if (pad_trig & 0x60) {
        if (sp2C == 1) {
            yazirushi_disp_switch(0);
            step_mode++;
            k_mode = 0;
            return;
        } else {
            k_disp_goto_line_end();
        }
    }
}
