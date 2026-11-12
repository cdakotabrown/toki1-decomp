/* @SymbolStart: 0x80134CCC */
/* @SymbolSize: 580 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x50:rt:v1:a1 0x54:rs:v1:a1 0x54:rt:v1:a1 0x58:rt:t6:v1 0x64:rt:t6:v1 0x78:rs:v1:a1 0x7c:rt:t7:t6 0x88:rt:t7:t6 0x94:rt:v1:a1 0x98:rt:v1:v0 0xa0:rt:t8:t7 0xa4:rs:t8:t7 0xa4:rt:t8:t7 0xac:rs:t8:t7 0xac:rt:t9:t8 0xb0:rs:v1:v0 0xb0:rt:t9:t8 0x11c:rt:t0:t9 0x128:rs:t0:t9 0x128:rt:t0:t9 0x130:rs:t0:t9 0x130:rt:t1:t0 0x134:rs:t1:t0 0x170:rt:v1:a1 0x174:rt:t2:t1 0x178:rs:t2:t1 0x178:rt:t2:t1 0x17c:rs:v1:a1 0x17c:rt:v1:a1 0x180:rd:t3:t2 0x180:rt:t2:t1 0x184:rd:t4:t3 0x184:rt:t3:t2 0x188:rs:t4:a1 0x188:rt:v1:t3 0x1a0:rt:t5:t4 0x1a4:rs:t5:t4 0x1a4:rt:t5:t4 0x1ac:rs:t5:t4 0x1b0:rt:t6:t5 0x1b4:rs:t6:t5 0x1b4:rt:t6:t5 0x1bc:rs:t6:t5 0x1c0:rt:t7:t6 0x1c4:rs:t7:t6 0x1c4:rt:t7:t6 0x1cc:rs:t7:t6 0x1f0:rt:t8:t7 0x204:rs:t8:t7 0x204:rt:t8:t7 0x20c:rs:t8:t7 */
/* tel_ring0 - TEL overlay, Address: 0x80134CD4 (true start 0x80134CCC) */
#include "types.h"

extern u8 now_girl;
extern u8 step_mode;
extern s32 work_fuku;
extern u8 tel_serifu_girl;
extern s32 step_counter;
extern s32 pad_trig;
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u32 reserved_date;

extern s32 tel_ring_str;
extern s32 tel_ring_again_str;

extern void sndi(s32, s32, s32);
extern void device_cmd_send(s32);
extern void k_sub_disp_start(s32);
extern s32 rand(void);
extern void k_sub_reset(void);
extern s32 reserved_date_check(s32, s32, s32);
extern void goto_sub_sub_step(s32);
extern s32 tel_cal_menu_set(void);
extern void sub_sub_step_up(void);
extern void menu_bar_show(s32);
extern void cal_base_show(void);
extern s32 k_disp_inc(void);

s32 tel_ring0(void)
{
    s32 a0;
    s32 v0;
    s32 counter;

    a0 = step_mode;

    if (a0 != 0) {
        goto state_1;
    }

    sndi((s32)&tel_ring_str, 0, 47);
    device_cmd_send(1288);
    k_sub_disp_start(0);
    v0 = rand();
    step_mode = 1;
    work_fuku = (v0 & 3) + 1;

    if (now_girl == 11) {
        tel_serifu_girl = 12;
    } else {
        tel_serifu_girl = now_girl;
    }
    goto end;

state_1:
    if (a0 != 1) {
        goto end;
    }

    if ((step_counter & 0x7F) != 1) {
        goto skip_decrement;
    }

    k_sub_reset();
    sndi((s32)&tel_ring_again_str, 0, 47);
    k_sub_disp_start(0);

    if (work_fuku > 0) {
        device_cmd_send(1288);
    }
    if (work_fuku == 0) {
        goto skip_decrement;
    }
    work_fuku = work_fuku - 1;

skip_decrement:
    counter = work_fuku;
    if (counter <= 0) {
        goto do_calls;
    }

    if ((pad_trig & 0x40) == 0) {
        goto end;
    }

do_calls:
    device_cmd_send(5384);
    device_cmd_send(1291);

    v0 = reserved_date_check(now_year, now_month, now_day);
    if (v0 != 0) {
        if (now_girl == ((reserved_date << 3) >> 28)) {
            goto_sub_sub_step(16);
            goto end;
        }
    }

    if (now_year == 98 && now_month == 2 && now_day == 22) {
        goto_sub_sub_step(16);
        goto end;
    }

    v0 = tel_cal_menu_set();
    if (v0 == 0) {
        goto_sub_sub_step(16);
        goto end;
    }

    if (tel_serifu_girl == 12) {
        goto_sub_sub_step(64);
        goto end;
    }

    sub_sub_step_up();

end:
    menu_bar_show(0);
    cal_base_show();
    return k_disp_inc();
}
