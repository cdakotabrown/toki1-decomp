/* @SymbolStart: 0x8013C8F4 */
/* @SymbolSize: 352 */
/* @RodataBase: 0x80140374 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x7c */
/* @PostProc: patch_insn_reg_field 0x70:rt:v0:v1 0x78:rs:v0:v1 0x78:rt:v0:v1 0x80:rd:v1:v0 0x80:rs:t9:v1 0x80:rt:v0:t9 0x84:rs:v1:v0 0x84:rt:t0:t1 0x8c:rs:t0:t1 0x8c:rt:t1:t2 0x90:rs:v1:v0 0x90:rt:t1:t2 0x94:rs:v0:v1 0x94:rt:t2:t3 0x9c:rs:t2:t3 0x9c:rt:t3:t4 0xa0:rs:t3:t4 0xa0:rt:t4:t5 0xa4:rs:v0:v1 0xa4:rt:t4:t5 0xd8:rt:t5:t6 0xdc:rs:t5:t6 0xdc:rt:t5:t6 0xe4:rs:t5:t6 0xe4:rt:t6:t7 0xec:rt:t6:t7 0x130:rt:t7:t8 0x134:rs:t7:t8 0x134:rt:t7:t8 0x13c:rs:t7:t8 0x13c:rt:t8:t9 0x144:rt:t8:t9 */
#include "types.h"

extern u8 step_mode;
extern u16 basyo_pack;
extern u8 screen_w[];
extern s32 tel_butyou_str;
extern s32 tel_taibu_str;
extern s32 p_sei;
extern s32 tel_taibu_p_str;

extern void xa_wait(void);
extern void k_sub_reset(void);
extern void gnsx(s32);
extern void sndisp(s32, s32, s32);
extern void xa_set_m(s32, s32, s32, s32);
extern void k_sub_disp_start(s32);
extern void xa_wait_m(void);
extern void sub_sub_step_up(void);

void tel_basyo_step(void)
{
    u8 *base;

    switch (step_mode) {
        case 0:
            xa_wait();
            break;
        case 1:
            k_sub_reset();
            gnsx((s32)&tel_butyou_str);
            base = (u8 *)((u32)screen_w | 0);
            base[(((u32)basyo_pack >> 12) * 4) + 1647] |= 1;
            base[245] = (base[245] & 0xFF0F) | 0xB0;
            sndisp((s32)&tel_taibu_str, 0, 13);
            xa_set_m(0x0D000020, 0x0400002F, -1, 14);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 2:
            xa_wait_m();
            break;
        case 3:
            k_sub_reset();
            gnsx((s32)&p_sei);
            sndisp((s32)&tel_taibu_p_str, 0, 15);
            k_sub_disp_start(0);
            step_mode = step_mode + 1;
            break;
        case 4:
            xa_wait();
            break;
        default:
            sub_sub_step_up();
            break;
    }
}
