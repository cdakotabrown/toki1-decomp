/* @RodataBase: 0x8013BCE8 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* @PostProc: patch_stack_complete --old 64 --new 80 */
/* @PostProc: remap_stack_slots --from 52 --to 44 --from 40 --to 72 --from 44 --to 68 --from 36 --to 60 */
/* @PostProc: patch_insn_reg_field 0x1f4:rd:a3:a1 0x214:rs:a3:a1 0x214:rt:a3:a1 0x218:rs:a3:a1 0x224:rd:a3:a1 0x22c:rt:a3:a1 0x238:rt:a3:a1 0x23c:rt:a0:v1 0x248:rt:a0:v1 0x24c:rt:v0:a3 0x250:rs:v0:a3 0x250:rt:v0:a3 0x254:rt:v1:a2 0x258:rs:v1:a2 0x258:rt:v1:a2 0x260:rt:v0:a3 0x268:rt:v1:a2 0x26c:rs:a0:v1 0x278:rt:a3:a1 0x2a0:rt:a3:a1 0x2a4:rt:a1:a2 0x2a8:rt:a2:a3 0x2b0:rs:a3:a1 0x2b0:rt:a3:a1 0x2b4:rt:a2:a3 0x2b8:rs:a3:a1 0x2bc:rt:a1:a2 */
/* @SymbolSize: 1376 */
/* Address: 0x80133F80, Size: 1376 bytes (344 instructions) */
/* 6. `@CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all` -- the volatile from (1) */
#include "types.h"

typedef struct {
    u16 v[5];
} MasterTbl5;

extern MasterTbl5 master_cw_judge1_tbl;
extern s32 master_cw_no;
extern s32 master_cw_idx;
extern volatile u8 master_cw_rank;
extern u8 master_cw_hit_tbl[5];
extern u8 now_sub_sub_step;
extern s16 para_kon;
extern u16 bukatu_pt6;
extern s16 now_serifu;
extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern s16 cal_cw_tick;
extern s16 cal_cw_anim;
extern s16 cal_cw_frame;
extern u8 cal_cw_clut;

extern void sub_sub_step_up(void);
extern void sub_step_up(void);
extern void serifu_run(void);
extern s32 rand(void);
extern void goto_sub_sub_step(s32);
extern void device_cmd_send(s32);
extern void k_reset(s32);
extern void game_reset(void);
extern void common_disp(void);

void master_cw_judge1(void) {
    MasterTbl5 tbl;
    s32 diff;
    s32 count;
    s32 i;
    s32 ra1, ra2;
    u8 e0, e4;
    u8 sa, sb;

    tbl = master_cw_judge1_tbl;
    switch ((u32)now_sub_sub_step) {
    case 0:
        now_serifu = master_cw_no * 3 + 1;
        sub_sub_step_up();
        break;
    case 1:
        serifu_run();
        break;
    case 2:
        if (rand() % 100 == 0) {
            goto_sub_sub_step(10);
        } else {
            sub_sub_step_up();
        }
        break;
    case 3:
        diff = (s16)(bukatu_pt6 + (para_kon >> 4) - tbl.v[master_cw_no]);
        if (diff < -30) {
            master_cw_rank = 0;
        } else if (diff < -20) {
            master_cw_rank = 1;
        } else if (diff < -10) {
            master_cw_rank = 1;
        } else if (diff < 0) {
            master_cw_rank = 2;
        } else if (diff >= 30) {
            master_cw_rank = 5;
        } else if (diff >= 20) {
            master_cw_rank = 4;
        } else if (diff >= 10) {
            master_cw_rank = 4;
        } else if (diff >= 0) {
            master_cw_rank = 3;
        }
        for (i = 0; i < 5; i++) {
            master_cw_hit_tbl[i] = 0;
        }
        for (i = 0; (u32)i < master_cw_rank; i++) {
            master_cw_hit_tbl[i] = 1;
        }
        count = rand() & 0xF;
        if ((u32)master_cw_rank >= 3U) {
            e4 = master_cw_hit_tbl[4];
            e0 = master_cw_hit_tbl[0];
            master_cw_hit_tbl[0] = e4;
            master_cw_hit_tbl[4] = e0;
        }
        for (i = 0; i < count; i++) {
            ra1 = rand() & 3;
            ra2 = rand() & 3;
            sa = master_cw_hit_tbl[ra1];
            sb = master_cw_hit_tbl[ra2];
            master_cw_hit_tbl[ra1] = sb;
            master_cw_hit_tbl[ra2] = sa;
        }
        cal_cw_clut = (u8)((master_cw_no + 2) % 5 + 16);
        master_cw_idx = 0;
        sub_sub_step_up();
        break;
    case 4:
        if (cal_cw_mode == 0) {
            if (master_cw_idx >= 5) {
                sub_sub_step_up();
            } else {
                cal_cw_disp = 132;
                cal_cw_mode = 5;
                cal_cw_frame = 0;
                cal_cw_tick = 0;
                if (master_cw_hit_tbl[master_cw_idx] == 1U) {
                    cal_cw_anim = 0;
                } else {
                    cal_cw_anim = 1;
                }
                master_cw_idx = master_cw_idx + 1;
            }
        }
        break;
    case 5:
        cal_cw_disp = 132;
        cal_cw_mode = 1;
        cal_cw_frame = 0;
        cal_cw_tick = 0;
        if ((u32)master_cw_rank >= 3U) {
            cal_cw_anim = 2;
        } else {
            cal_cw_anim = 3;
            if (master_cw_no == 3) {
                now_serifu = 23;
            } else {
                now_serifu = 25;
            }
        }
        if ((u32)master_cw_rank >= 3U) {
            device_cmd_send(516);
        } else {
            device_cmd_send(517);
        }
        sub_sub_step_up();
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        k_reset(1);
        sub_sub_step_up();
        break;
    case 8:
        serifu_run();
        break;
    case 9:
        cal_cw_disp = 0;
        cal_cw_mode = 0;
        sub_step_up();
        break;
    case 10:
        now_serifu = 16;
        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        k_reset(1);
        now_serifu = now_serifu + master_cw_no;
        sub_sub_step_up();
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        device_cmd_send(516);
        now_serifu = 22;
        sub_sub_step_up();
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        master_cw_rank = 3;
        sub_step_up();
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
}
