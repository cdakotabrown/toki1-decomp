/* @SymbolStart: 0x801474E8 */
/* @SymbolSize: 308 */
/* @RodataBase: 0x8014D800 */
/* @PostProc: patch_insn_reg_field 0x40:rt:v0:t9 0x48:rd:v0:t9 0x48:rs:v0:t9 0x4c:rs:v0:t9 0x4c:rt:v0:t9 0x54:rs:v0:t9 0x5c:rt:v0:t9 */
#include "types.h"

extern void k_reset(s32);
extern void gnsx(void *);
extern void sndisp(void *, s32, s32);
extern void k_disp_inc(void);
extern s32  check_end_k(void);
extern void sub_sub_step_up(void);

extern u8  p_sei[];
extern u8  now_youbi;
extern u8  etc_week_grade_disp_str0[];
extern u8  etc_week_grade_disp_str1[];
extern u8  etc_week_grade_disp_str2[];
extern u8  etc_week_grade_disp_str3[];
extern u8  etc_week_grade_disp_str4[];
extern u8  step_mode;
extern s32 work_fuku[];

void etc_week_grade_disp(void)
{
    s32 gate = step_mode;

    s32 idx;

    if (gate == 0) {
        k_reset(1);
        gnsx(p_sei);
        idx = (&work_fuku[4])[now_youbi];
        switch (idx) {
        case 0:
            sndisp(etc_week_grade_disp_str0, 0, 31);
            break;
        case 1:
            sndisp(etc_week_grade_disp_str1, 0, 31);
            break;
        case 2:
            sndisp(etc_week_grade_disp_str2, 0, 31);
            break;
        case 3:
            sndisp(etc_week_grade_disp_str3, 0, 31);
            break;
        case 4:
            sndisp(etc_week_grade_disp_str4, 0, 31);
            break;
        }
        step_mode++;
    } else {
        k_disp_inc();
        if (check_end_k() != 0)
            sub_sub_step_up();
    }
}
