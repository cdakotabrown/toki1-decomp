/* @SymbolStart: 0x801335E0 */
/* @DataBase: 0x80134350 */
/* @RodataBase: 0x801341E8 */
/* @SymbolSize: 872 */
/* @PostProc: swap_instructions --offset1 0x9c --offset2 0xa4 */
/* @PostProc: patch_insn_reg_field 0x18c:rt:t5:t4 0x190:rd:t3:t2 0x194:rd:t4:t3 0x194:rt:t3:t2 0x198:rd:t5:t4 0x198:rs:t5:t4 0x198:rt:t4:t3 0x19c:rs:t5:t4 0x19c:rt:t5:t4 0x1a4:rs:t5:t4 0x1a4:rt:t8:t5 0x1a8:rs:t8:t5 0x1a8:rt:t7:t8 0x1ac:rt:t7:t8 0x1d8:rt:t6:t7 0x1dc:rs:t6:t7 0x1dc:rt:t6:t7 0x1e0:rt:t1:t0 0x1e4:rd:t9:t6 0x1e4:rt:t6:t7 0x1e8:rd:t0:t9 0x1e8:rt:t9:t6 0x1ec:rs:t1:t0 0x1ec:rt:t1:t0 0x1f0:rd:v0:v1 0x1f0:rs:t0:t9 0x1f0:rt:t1:t0 0x1f4:rs:v0:v1 0x1f4:rt:t3:t2 0x1f8:rs:v0:v1 0x1f8:rt:t2:t1 0x1fc:rs:t3:t2 0x1fc:rt:t4:t3 0x200:rd:t5:t4 0x200:rt:t4:t3 0x204:rd:t8:t5 0x204:rt:t5:t4 0x208:rs:t8:t5 0x208:rt:t7:t8 0x20c:rt:t0:t9 0x210:rd:v1:v0 0x210:rs:t2:t1 0x210:rt:t7:t8 0x214:rd:t6:t7 0x214:rt:v1:v0 0x218:rd:v1:v0 0x218:rt:t6:t7 0x21c:rs:v1:v0 0x22c:rt:t0:t9 0x230:rs:v1:v0 0x238:rt:t1:t0 0x244:rt:t1:t0 0x248:rs:v1:v0 0x250:rt:t3:t2 0x25c:rt:t3:t2 0x260:rs:v1:v0 0x264:rs:v1:v0 0x268:rt:t4:t3 0x274:rt:t4:t3 0x27c:rt:t5:t4 0x288:rt:t5:t4 0x28c:rs:v1:v0 0x294:rt:t8:t5 0x2a0:rt:t8:t5 0x2a4:rs:v1:v0 0x2ac:rt:t2:t1 0x2b8:rt:t2:t1 0x2bc:rs:v1:v0 0x2c0:rt:t7:t8 0x2c8:rt:t7:t8 0x2fc:rt:t6:t7 0x300:rt:t6:t7 0x304:rs:t6:t7 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_cmd_send(u32 a0);
extern void k_reset(u32 a0);
extern void sub_step_up(void);
extern void common_disp(void);
extern void serifu_run(void);
extern void fade_out_step(void);
extern void class_clear_n(u32 a0);
extern void goto_sub_step(u32 a0);

typedef struct {
    u16 hwords[11];
} StructCopy26;


extern u8  now_sub_sub_step;
extern u8  cal_cw_mode;
extern u8  cal_cw_disp;
extern u8  cal_cw_bright;
extern s16 cal_cw_tick;
extern s16 cal_cw_anim;
extern s16 cal_cw_frame;
extern u8  rensyu_trial_no;
extern u8  rensyu_succ_cnt;
extern u8  rensyu_trial_hit_flag[];
extern u8  rensyu_girl_no;
extern u16 basyo_pack;
extern u16 bukatu_rec_hi[];
extern u8  screen_w[];
extern s16 now_serifu;

void rensyu_trial_run_main(void)
{
    u32 v0;
    s16 diff;
    StructCopy26 local = { {
        30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330
    } };
    u8 *bp;


    switch (now_sub_sub_step) {
    case 0:
        if (cal_cw_mode != 0) {
            break;
        }
        v0 = rensyu_trial_no;
        if (v0 >= 5U || (v0 != 0 && ((u32)basyo_pack >> 12) == 9)) {
            sub_sub_step_up();
            break;
        }
        cal_cw_disp = 132;
        cal_cw_mode = 5;
        cal_cw_frame = 0;
        cal_cw_tick = 0;
        if (rensyu_trial_hit_flag[v0] == 1) {
            cal_cw_anim = 0;
        } else {
            cal_cw_anim = 1;
        }
        rensyu_trial_no++;
        break;
    case 1:
        if ((u32)rensyu_succ_cnt >= 3U) {
            device_cmd_send(516);
            now_serifu = 17;
        } else {
            device_cmd_send(517);
            now_serifu =
                (bukatu_rec_hi[((u32)basyo_pack >> 12) * 2] & 0xF) + 18;
        }
        k_reset(1);
        sub_sub_step_up();
        break;
    case 2:
        serifu_run();
        break;
    case 3:
        bp = screen_w + ((u32)basyo_pack >> 12) * 4;
        diff = (s16)(*(u16 *)(bp + 1644) -
                     local.hwords[*(u16 *)(bp + 1646) & 0x0F]);
        if (diff < -30) {
            now_serifu = 29;
        } else if (diff < -20) {
            now_serifu = 30;
        } else if (diff < -10) {
            now_serifu = 31;
        } else if (diff < 0) {
            now_serifu = 32;
        } else if (diff >= 30) {
            now_serifu = 36;
        } else if (diff >= 20) {
            now_serifu = 35;
        } else if (diff >= 10) {
            now_serifu = 34;
        } else if (diff >= 0) {
            now_serifu = 33;
        }
        sub_sub_step_up();
        break;
    case 4:
        serifu_run();
        break;
    case 5:
        v0 = (cal_cw_bright - 4) & 0xFF;
        cal_cw_bright = v0;
        if (v0 < 3) {
            cal_cw_bright = 0;
        }
        fade_out_step();
        break;
    case 6:
        class_clear_n(96);
        v0 = rensyu_girl_no;
        if (v0 == 11 || v0 == 255) {
            goto_sub_step(7);
        } else {
            sub_step_up();
        }
        break;
    default:
        break;
    }

    common_disp();
}
