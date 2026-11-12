/* @SymbolStart: 0x80149EC8 */
/* @SymbolSize: 568 */
/* @CompileEnv: IDO52_EXCLUDE_REGS_WEB=24:v0,3:a1 IDO52_FORCE_WEB_COLOR=22:a2,49:a2,61:a2 */
/* Address: 0x80149ED8 (true start 0x80149EC8: the etc_siken_qno load and the */
/* etc_bustup_reset.c already scopes away with @SymbolSize: 224). */
/* padded with `@PostProc: pad_trailing_nop --count 138`. */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0xb8 */
/* @PostProc: swap_instructions --offset1 0xb4 --offset2 0xb8 */
/* @PostProc: patch_insn_reg_field 0x60:rt:a2:v0 0x64:rs:a2:v0 0x64:rt:a2:v0 0x6c:rt:a2:v0 0xb4:rt:a2:v0 0xb8:rs:a2:v0 0xb8:rt:a2:v0 0xc0:rt:a2:v0 0x128:rt:a2:v0 0x12c:rs:a2:v0 0x12c:rt:a2:v0 0x138:rt:a2:v0 */
#include "types.h"

extern void printf();

extern u8 byouki_kind;
extern volatile s16 para_bun;
extern volatile s16 para_ri;
extern volatile s16 para_gei;
extern u16 bukatu_rec;
extern u16 bukatu_pt2;
extern u16 bukatu_pt4;
extern u8 step_mode;
extern u8 cal_cw_mode;
extern s16 cal_cw_tick;
extern s16 cal_cw_anim;
extern s16 cal_cw_frame;
extern u8 etc_siken_cw_judge_str0[];
extern volatile s32 etc_siken_qno;
extern s32 etc_siken_sel;
extern s32 etc_siken_seikai;
extern s32 etc_siken_base;
extern s32 etc_siken_thr_tbl[];

void etc_siken_cw_judge(void)
{
    s32 n;
    s32 flag;
    s32 val;
    s32 cnt;
    s32 amt;
    s32 mode;

    flag = 0;
    n = etc_siken_qno;
    switch (n % 3) {
    case 0:
        cnt = (s32)((u32)bukatu_rec >> 4);
        val = para_bun;
        amt = cnt;
        if (cnt > 0) {
            if (cnt >= 21) {
                amt = 20;
            }
        }
        val = val + amt;
        if ((byouki_kind & 0xF) != 0) {
            val = val / 2;
        }
        break;

    case 1:
        cnt = (s32)((u32)bukatu_pt2 >> 4);
        val = para_ri;
        amt = cnt;
        if (cnt > 0) {
            if (cnt >= 21) {
                amt = 20;
            }
        }
        val = val + amt;
        if ((byouki_kind & 0xF) != 0) {
            val = val / 2;
        }
        break;

    default:
        printf(etc_siken_cw_judge_str0);
        n = etc_siken_qno;

    case 2:
        cnt = (s32)((u32)bukatu_pt4 >> 4);
        val = para_gei;
        amt = cnt;
        if (cnt > 0) {
            if (cnt >= 21) {
                amt = 20;
            }
        }
        val = val + amt;
        if ((byouki_kind & 0xF) != 0) {
            val = val / 2;
        }
        break;
    }

    mode = etc_siken_sel;
    n = n + 1;
    if (mode == 1) {
        val = val + 38;
    }
    if (mode == 2) {
        val = val + 54;
    }
    etc_siken_qno = n;
    if (val < etc_siken_thr_tbl[(etc_siken_base + n) % 12]) {
        flag = 1;
    }
    if (flag == 0) {
        etc_siken_seikai = etc_siken_seikai + 1;
    }
    cal_cw_mode = 5;
    cal_cw_tick = 0;
    cal_cw_anim = flag;
    cal_cw_frame = 0;
    step_mode = step_mode + 1;
}
