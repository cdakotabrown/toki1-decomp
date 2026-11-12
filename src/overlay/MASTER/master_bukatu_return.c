/* @SymbolStart: 0x8013B20C */
/* @SymbolSize: 328 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 0x40:rt:v1:v0 0x44:rs:v1:v0 0x110:rt:t5:t6 0x118:rs:t5:t6 0x118:rt:t6:t7 0x11c:rt:t6:t7 */
/* master_bukatu_return - MASTER overlay, Address: 0x8013B214, Size: 320 bytes (80 insns) */
#include "types.h"

extern u8 cal_cw_bright;
extern u8 master_bukatu_girl_no;
extern u8 day_flag;
extern u8 master_bukatu_rank;
extern u16 basyo_pack;
extern u8 screen_w;

extern void fade_out_step(void);
extern void fade_out(void);
extern void device_cmd_send(s32);
extern void season_bgm_set(void);
extern void sub_sub_step_up(void);
extern s32 device_load_poll(void);
extern void shared_system_state_reset(void);
extern void set_dec_bri(s32);
extern void return_schedule(s32);
extern void game_reset(void);
extern void common_disp(void);
extern u8 now_sub_sub_step;

void master_bukatu_return(void) {
    s32 sel;
    u8 vol;
    u32 padidx;
    u8 *entry;

    sel = now_sub_sub_step;

    switch (sel) {
    case 0:
        vol = cal_cw_bright;
        if ((u32)vol < 4) {
            cal_cw_bright = 0;
        } else {
            cal_cw_bright = (u8)(vol - 4);
        }
        if (master_bukatu_girl_no == 11) {
            fade_out_step();
        } else {
            fade_out();
        }
        break;
    case 1:
        device_cmd_send(191);
        season_bgm_set();
        sub_sub_step_up();
        break;
    case 2:
        if (device_load_poll() == 1) {
            day_flag = (u8)(day_flag | 0x10);
            shared_system_state_reset();
            if ((u32)master_bukatu_rank >= 3) {
                padidx = (u32)basyo_pack >> 12;
                entry = &screen_w + padidx * 4;
                entry[1647] = (u8)(entry[1647] | 0x40);
            }
            set_dec_bri(0);
            return_schedule(1);
        }
        break;
    default:
        game_reset();
        break;
    }

    common_disp();
}
