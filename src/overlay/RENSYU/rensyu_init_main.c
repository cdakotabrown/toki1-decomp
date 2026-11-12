/* @SymbolStart: 0x801320E0 */
/* @SymbolSize: 688 */
/* @RodataBase: 0x801340E4 */
/* @LiteralBase: 0x801340D0 */
/* @PostProc: patch_insn_reg_field 0x174:rt:v0:t0 0x178:rs:v0:t0 0x178:rt:v0:t0 0x17c:rt:v1:at 0x188:rt:a0:t3 0x18c:rt:v0:t0 0x190:rs:a0:t3 0x190:rt:a0:t3 0x194:rt:a1:at 0x1a0:rt:v1:t1 0x1a8:rd:t0:t4 0x1a8:rt:a0:t3 0x1ac:rs:t0:t4 0x1ac:rt:t1:t5 0x1b0:rt:a1:t2 0x1b4:rs:t1:t5 0x1bc:rd:t1:t5 0x1bc:rt:t1:t5 0x1c4:rt:t1:t5 0x1c8:rt:t1:t5 0x1d0:rs:t1:t5 0x1e4:rt:t2:t6 0x1f0:rt:t2:t6 0x1f4:rt:t3:t7 0x200:rt:t3:t7 0x204:rt:t4:t8 0x210:rt:t4:t8 0x214:rt:t5:t9 0x220:rt:t5:t9 0x250:rt:a0:t0 0x254:rs:a0:t0 0x254:rt:a0:t0 0x258:rt:v0:t3 0x25c:rd:t7:t1 0x25c:rt:a0:t0 0x260:rd:t8:t2 0x260:rt:t7:t1 0x264:rd:v0:t3 0x264:rs:v0:t3 0x264:rt:t8:t2 0x268:rs:v0:t3 0x268:rt:v0:t3 0x270:rs:v0:t3 0x270:rt:t9:t4 0x274:rs:t9:t4 0x274:rt:t0:t5 0x278:rt:t0:t5 */
/* @PostProc: swap_instructions --offset1 0x17c --offset2 0x184 */
/* @PostProc: swap_instructions --offset1 0x180 --offset2 0x184 */
/* @PostProc: swap_instructions --offset1 0x194 --offset2 0x19c */
/* @PostProc: swap_instructions --offset1 0x198 --offset2 0x19c */
/* @PostProc: patch_insn_reg_field 0x180:rt:at:t1 0x184:rs:v1:t1 0x184:rt:v1:t1 0x198:rt:at:t2 0x19c:rs:a1:t2 0x19c:rt:a1:t2 */
/* Address: 0x801320E8 (true start 0x801320E0) */
#include "types.h"
#include "overlay_text/RENSYU.h"

extern void device_cmd_send(u32 a0);
extern void device_load_start(u32 a0, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5);
extern void date_x_taku_set(u32 a0, u32 a1);
extern void shared_system_state_reset(void);
extern void message_window_init(void);
extern void icon_disp_switch(u32 a0);
extern void mascot_disp_switch(u32 a0);
extern void parameter_disp_switch(u32 a0);
extern void hizuke_init(void);
extern void k_reset(u32 a0);
extern void basyo_init(u32 a0);
extern void mes_window_switch(u32 a0);
extern void dec_bg_reset(void);
extern void set_dec_bri(u32 a0);
extern void srand(u32 a0);
extern void sub_sub_step_up(void);
extern u32  device_load_poll(void);
extern void cd_read_start2(u32 a0, u32 a1, u32 a2);
extern u32  get_CD_State2(void);
extern void date_name_set(u32 a0);
extern void yazirushi_off(void);
extern void rensyu_serifu_tbl_set(void);
extern void game_reset(void);
extern void common_disp(void);
extern void serifu_run(void);
extern void sub_step_up(void);

extern u8 now_sub_sub_step;
extern volatile u32 rensyu_serifu_ptr;
extern volatile u32 rensyu_serifu_kind_ptr;
extern volatile u32 rensyu_sound_ptr;
extern volatile u32 ss_serifu;
extern volatile u32 ss_serifu_kind;
extern volatile u32 sss_sound;
extern volatile u16 basyo_pack;
extern volatile u16 bukatu_rec_hi[];
extern volatile u16 now_serifu;
extern volatile u16 now_serifu_in;
extern volatile u32 vblnk_counter;

void rensyu_init_main(void)
{
    u32 a;
    u32 b;
    u32 c;
    u32 val;
    u32 idx;
    u32 entry;

    switch (now_sub_sub_step) {
    case 0:
        device_cmd_send(191);
        device_load_start(0, 0xBFA6, 0xBF87, 0xC990, 0xC943, 0xC929);
        date_x_taku_set(1, 0);
        shared_system_state_reset();
        message_window_init();
        icon_disp_switch(0);
        mascot_disp_switch(0);
        parameter_disp_switch(0);
        hizuke_init();
        k_reset(0);
        basyo_init((u32)TXT_801340D0);
        mes_window_switch(0);
        dec_bg_reset();
        set_dec_bri(0);
        srand(vblnk_counter);
        sub_sub_step_up();
        break;
    case 1:
        if (device_load_poll() == 1) {
            device_cmd_send(518);
            sub_sub_step_up();
        }
        break;
    case 2:
        cd_read_start2(7, 0x801E8000, 0xB037);
        sub_sub_step_up();
        break;
    case 3:
        if (get_CD_State2() & 1) {
            date_name_set(0);
            yazirushi_off();
            rensyu_serifu_tbl_set();
            a = rensyu_serifu_ptr;
            b = rensyu_serifu_kind_ptr;
            ss_serifu = a;
            val = basyo_pack;
            c = rensyu_sound_ptr;
            ss_serifu_kind = b;
            idx = (val >> 12) - 6;
            sss_sound = c;
            switch (idx) {
            case 0:
                now_serifu = 0;
                break;
            case 1:
                now_serifu = 1;
                break;
            case 2:
                now_serifu = 2;
                break;
            case 3:
                now_serifu = 3;
                break;
            case 4:
                now_serifu = 4;
                break;
            default:
                game_reset();
                break;
            }
            now_serifu_in = 0;
            sub_sub_step_up();
        }
        break;
    case 4:
    case 6:
        serifu_run();
        break;
    case 5:
        val = basyo_pack;
        idx = val >> 12;
        entry = bukatu_rec_hi[idx * 2];
        now_serifu = (entry & 0xF) + 5;
        now_serifu_in = 0;
        sub_sub_step_up();
        break;
    case 7:
        sub_step_up();
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
}
