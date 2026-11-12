/* @CompileEnv: IDO52_EXCLUDE_REGS_WEB=32:v0+v1+a0 */
/* @SymbolSize: 592 */
/* @PostProc: patch_stack_complete --old 64 --new 80 */
/* @PostProc: swap_instructions --offset1 0x154 --offset2 0x160 */
/* @PostProc: patch_insn_reg_field 0x48:rt:v1:v0 0x4c:rs:v1:v0 0x4c:rt:v1:v0 0x54:rs:v1:v0 0x60:rs:v1:v0 0x68:rs:v1:v0 0xa4:rt:a1:v1 0xac:rt:a1:v1 0xd0:rt:v1:v0 0xd4:rt:s7:fp 0xdc:rs:v1:v0 0xe0:rd:s7:fp 0xe0:rs:s7:fp 0xe4:rs:s7:fp 0xe4:rt:s7:fp 0x100:rt:a1:v1 0x104:rs:a1:v1 0x108:rt:fp:s2 0x10c:rt:fp:s2 0x114:rt:s6:s7 0x118:rt:s2:s3 0x11c:rs:s2:s3 0x11c:rt:s2:s3 0x120:rs:s6:s7 0x120:rt:s6:s7 0x124:rt:s5:s6 0x128:rt:s4:s5 0x12c:rt:s3:s4 0x130:rd:t1:t2 0x134:rs:t1:fp 0x134:rt:s7:t2 0x138:rt:t2:t3 0x13c:rs:t2:t3 0x13c:rt:t2:t3 0x140:rt:t3:t4 0x144:rd:a1:v1 0x148:rs:t2:t3 0x148:rt:t3:t4 0x158:rs:v0:s3 0x158:rt:s2:v0 0x15c:rt:s3:s4 0x160:rt:v1:v0 0x164:rt:v1:v0 0x16c:rs:fp:s2 0x170:rd:t4:t5 0x178:rt:s4:s5 0x17c:rs:t4:t5 0x190:rt:v0:t6 0x19c:rt:v0:t6 0x1a0:rt:t6:t8 0x1a4:rd:t5:t7 0x1a4:rs:v0:t6 0x1a4:rt:s5:s6 0x1a8:rs:t5:t7 0x1b0:rs:t6:t8 0x1b0:rt:t6:t8 0x1b8:rd:t7:t9 0x1b8:rt:t6:t8 0x1bc:rd:t8:t0 0x1bc:rs:s6:s7 0x1bc:rt:t7:t9 0x1c0:rs:t8:t0 0x1cc:rt:t9:t1 0x1d4:rt:t9:t1 0x1dc:rt:t0:t2 0x1e4:rt:t0:t2 0x1e8:rs:fp:s2 0x1ec:rt:a1:v1 0x1f4:rt:fp:s2 0x1fc:rd:t1:t2 0x200:rt:t1:t3 0x204:rs:t1:t3 0x204:rt:t1:t3 0x20c:rt:t3:t5 0x210:rs:t1:t3 0x210:rt:t2:t4 0x214:rs:t3:t5 0x214:rt:t3:t5 0x218:rt:t2:t4 0x220:rs:t3:t5 */
#include "types.h"

extern void k_reset(s32);
extern void sub_sub_step_up(void);
extern void sprite_flag_set(s32, s32, s32);
extern void kanji_str_disp2(s32 a0_in);
extern void bunka_sd_list_disp3(void);

extern s32 step_state;
extern s32 step_counter;
extern s32 bunka_sd_rec2_pos;
extern s32 bunka_sd_rec2_page_no;
extern s32 bunka_sd_rec2_frame_cnt;
extern s32 bunka_sd_rec2_phase_no;
extern s32 bunka_sd_rec2_title_flag;
extern s32 bunka_sd_rec2_scr_tbl[];
extern s32 bunka_sd_rec2_title_tbl[];

s32 bunka_sd_rec_play2(void)
{
    s32 phase;
    s32 phase2;
    s32 i;
    s32 recBase;
    s32 *rec;
    s32 flags;
    s32 hi;
    s32 lo;
    s32 tmp;

    if (step_state == 0) {
        phase = bunka_sd_rec2_phase_no;
        bunka_sd_rec2_title_flag = 0;
        switch ((u32)phase) {
        case 0:
            break;
        case 1:
            step_counter = 0;
            break;
        case 2:
            bunka_sd_rec2_page_no = bunka_sd_rec2_page_no + 1;
            bunka_sd_rec2_frame_cnt = 0;
            bunka_sd_rec2_phase_no = 0;
            bunka_sd_rec2_pos = 1;
            k_reset(1);
            sub_sub_step_up();
            return 1;
        }
    }

    recBase = bunka_sd_rec2_scr_tbl[bunka_sd_rec2_page_no];
    phase2 = bunka_sd_rec2_phase_no;
    if (phase2 == 0) {
        phase2 = 1;
    }
    bunka_sd_rec2_phase_no = phase2;

    if (phase2 == 1) {
        i = bunka_sd_rec2_pos;
        if (i < 0xFFFF) {
            do {
                rec = (s32 *)(recBase + i * 8);
            if (bunka_sd_rec2_frame_cnt == rec[0]) {
                flags = rec[1];
                if (flags == 0x0FFFFFFF) {
                    bunka_sd_rec2_phase_no = 2;
                    i = 0xFFFF;
                } else {
                    hi = (flags & 0x00FF0000) >> 16;
                    lo = flags & 0x00FF;
                    if (hi) {}
                    if (lo == 254) {
                        sprite_flag_set(hi, 80, 0);
                    } else {
                        sprite_flag_set(hi, lo, 1);
                    }
                    tmp = rec[1];
                    if (tmp & 0xF0000000) {
                        kanji_str_disp2(bunka_sd_rec2_title_tbl[bunka_sd_rec2_page_no]);
                        step_state = 1;
                        bunka_sd_rec2_title_flag = 1;
                    }
                }
            } else {
                bunka_sd_rec2_pos = i;
                i = 0xFFFF;
                }
                i++;
            } while (i < 0xFFFF);
        }
    }

    bunka_sd_rec2_frame_cnt = bunka_sd_rec2_frame_cnt + 1;
    if (bunka_sd_rec2_title_flag == 1) {
        bunka_sd_list_disp3();
    }
}
