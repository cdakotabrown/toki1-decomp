/* @SymbolStart: 0x8013731C */
/* Address: 0x80137324 */
/* Size: 172 bytes (43 insns) */
/* @SymbolSize: 180 */
/* @PostProc: patch_insn_reg_field 0x70:rt:v0:t1 0x7c:rs:v0:t1 0x7c:rt:v0:t1 0x80:rt:t1:t3 0x84:rs:v0:t1 0x84:rt:v0:t2 0x88:rt:v0:t2 0x90:rt:t1:t3 */
#include "types.h"

extern void bunka_sd_title_disp(s32 a0_in);
extern void bunka_sd_list_disp(void);
extern void k_reset(s32);
extern void sub_sub_step_up(void);

extern s32 step_state;
extern s32 step_counter;
extern s32 bunka_sd_rec_pos;
extern s32 bunka_sd_rec_page_no;
extern s32 bunka_sd_rec_frame_cnt;
extern s32 bunka_sd_rec_phase_no;
extern s32 bunka_sd_rec_title_tbl[];

void bunka_sd_page_next(void)
{
    s32 t6_hidden;
    s32 counter;

    t6_hidden = step_counter;

    if (t6_hidden == 1) {
        bunka_sd_title_disp(bunka_sd_rec_title_tbl[bunka_sd_rec_page_no]);
        step_state = 1;
        bunka_sd_list_disp();
        return;
    }

    bunka_sd_list_disp();
    if (step_state == 0) {
        counter = bunka_sd_rec_page_no;
    } else {
        return;
    }
    counter = counter + 1;
    bunka_sd_rec_page_no = counter;
    bunka_sd_rec_pos = 1;
    bunka_sd_rec_frame_cnt = 0;
    bunka_sd_rec_phase_no = 0;
    k_reset(1);
    sub_sub_step_up();
}
