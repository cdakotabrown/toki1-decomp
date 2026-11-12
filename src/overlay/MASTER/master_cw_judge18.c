/* @CompileEnv: IDO52_COPYPROP_SITE=lod:9 */
/* @SymbolStart: 0x80135FB8 */
/* @SymbolSize: 1084 */
/* @RodataBase: 0x8013BE44 */
/* @PostProc: patch_stack_complete --old 96 --new 72 */
/* @PostProc: remap_stack_slots --from 40 --to 60 */
/* Address: 0x80135FC0 (window starts at the absorbed cross-prologue head */
/* Size: 1076 bytes (269 insns) + 8-byte head = 1084 */
/* @PostProc: swap_instructions --offset1 0x160 --offset2 0x164 */
/* @PostProc: swap_instructions --offset1 0x2d8 --offset2 0x2dc */
/* @PostProc: patch_insn_reg_field 0x1a0:rt:t2:a2 0x1a4:rs:t2:a2 0x1a4:rt:t2:a2 0x1a8:rt:v0:a1 0x1ac:rs:v0:a1 0x1ac:rt:v0:a1 0x1b4:rt:t2:a2 0x1bc:rt:v0:a1 0x1dc:rt:a1:t2 0x1e0:rd:a0:v1 0x1e4:rd:v1:a0 0x1e4:rt:a1:t2 0x1e8:rs:a0:v1 0x1e8:rt:a2:a1 0x1ec:rs:v1:a0 0x1ec:rt:a3:a2 0x1f8:rs:a0:v1 0x1f8:rt:a3:a2 0x200:rs:v1:a0 0x200:rt:a2:a1 0x22c:rt:a0:v0 0x238:rs:a0:v0 0x238:rt:a0:v0 0x240:rs:a0:v0 0x284:rt:a0:v0 0x288:rt:v0:t9 0x290:rt:v0:t9 0x294:rs:a0:v0 0x294:rt:a1:v0 0x2a4:rt:t9:t0 0x2ac:rt:t9:t0 0x2b8:rt:a1:v0 0x2bc:rt:t0:t1 0x2c4:rt:t0:t1 0x2cc:rt:t1:t2 0x2d0:rt:t1:t2 0x2dc:rt:v0:t3 0x2e0:rs:v0:t3 0x2e0:rt:v0:t3 0x2ec:rs:v0:t3 0x300:rt:t2:t4 0x308:rt:t2:t4 0x310:rt:t3:t5 0x318:rt:t3:t5 0x324:rt:t4:t6 0x32c:rt:t4:t6 0x334:rt:t5:t7 0x338:rt:t5:t7 0x35c:rt:v0:t8 0x360:rs:v0:t8 0x360:rt:v0:t8 0x364:rt:t6:t9 0x368:rs:v0:t8 0x370:rt:t7:t0 0x37c:rt:t6:t9 0x384:rt:t7:t0 0x3b0:rt:v0:v1 0x3b4:rs:v0:v1 0x3b4:rt:v0:v1 0x3bc:rs:v0:v1 0x3bc:rt:v0:v1 0x3c0:rs:v0:v1 0x3c0:rt:t8:t1 0x3c4:rt:t8:t1 0x3c8:rs:t8:t1 0x3d0:rt:t9:v1 0x3dc:rt:t9:v1 0x3f4:rt:t0:t2 0x3f8:rs:t0:t2 0x3f8:rt:t0:t2 0x400:rd:t1:t3 0x400:rt:t0:t2 0x404:rd:t2:t4 0x404:rt:t1:t3 0x408:rs:t2:t4 */
#include "types.h"

extern s32 rand(void);
extern void sub_sub_step_up(void);
extern void serifu_run(void);
extern void fade_out_step(void);
extern void device_cmd_send(s32);
extern void sub_step_up(void);
extern void game_reset(void);
extern void common_disp(void);
extern u8  now_sub_sub_step;
extern s16 para_kon;
extern u16 bukatu_pt6;
extern u32 bukatu_pt6_w;
extern s16 now_serifu;
extern u8  cal_cw_mode;
extern u8  cal_cw_disp;
extern u8  cal_cw_clut;
extern s16 cal_cw_tick;
extern s16 cal_cw_anim;
extern s16 cal_cw_frame;
extern u8  cal_cw_bright;
extern s32 master_cw_idx;
extern volatile u8 master_cw_rank;
extern u8  master_cw_hit_tbl[];
extern u8  D_8013C57C_b[];
extern u8  D_8013C57C_c[];
extern u8  D_8013C57C_d[];
extern u8  master_cw_hit_tbl_4;
extern u8  master_cw_hit_tbl_end[];

void master_cw_judge18(void) {
    s32 sel = now_sub_sub_step;
    s16 cam_y;
    u16 hair_y;
    s32 diff;
    s32 s0;
    u8 diridx;
    u8 tmp1, tmp2;
    s32 s1v, t2v;
    u8 a, b;
    s32 idx;
    u8 tval;
    u8 didx;
    u32 flags;
    u32 rot;
    u8 step;
    s32 v1rand;
    u8 *v0ptr;
    u8 *v1end;
    u8 *s2base;
    u8 *pb;

    switch ((u32)sel) {
    case 0:
        cam_y = para_kon;
        hair_y = bukatu_pt6;

        diff = (s32)(s16)(((s32)cam_y >> 4) + (s32)hair_y - 320);
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

        v0ptr = master_cw_hit_tbl;
        v1end = master_cw_hit_tbl_end;
        do {
            *v0ptr = 0;
            v0ptr++;
        } while (v0ptr < v1end);

        s0 = 0;
        if (master_cw_rank != 0) {
            v0ptr = D_8013C57C_b;
            do {
                *v0ptr = 1;
                s0++;
                v0ptr++;
            } while ((u32)s0 < (u32)master_cw_rank);
        }

        s0 = 0;
        v1rand = rand() & 0xF;
        if ((u32)master_cw_rank >= 3U) {
            tmp1 = master_cw_hit_tbl_4;
            tmp2 = D_8013C57C_c[0];
            D_8013C57C_c[0] = tmp1;
            master_cw_hit_tbl_4 = tmp2;
        }

        if (v1rand > 0) {
            s2base = D_8013C57C_d;
            do {
                s1v = rand() & 3;
                t2v = rand() & 3;
                pb = &s2base[t2v];
                a = s2base[s1v];
                b = *pb;
                s0++;
                s2base[s1v] = b;
                *pb = a;
            } while (s0 != v1rand);
        }

        cal_cw_clut = 18;
        master_cw_idx = 0;
        sub_sub_step_up();
        break;

    case 1:
        step = cal_cw_mode;
        if (step == 0) {
            idx = master_cw_idx;
            s2base = D_8013C57C_d;
            if (idx >= 5) {
                sub_sub_step_up();
            } else {
                cal_cw_disp = 132;
                cal_cw_mode = 5;
                cal_cw_frame = 0;
                cal_cw_tick = 0;
                tval = s2base[idx];
                idx = idx + 1;
                if (tval == 1U) {
                    cal_cw_anim = 0;
                } else {
                    cal_cw_anim = 1;
                }
                master_cw_idx = idx;
            }
        }
        break;

    case 2:
        cal_cw_disp = 132;
        cal_cw_mode = 1;
        didx = master_cw_rank;
        cal_cw_frame = 0;
        cal_cw_tick = 0;
        if ((u32)didx >= 3U) {
            device_cmd_send(516);
            cal_cw_anim = 2;
            now_serifu = 14;
        } else {
            device_cmd_send(517);
            cal_cw_anim = 3;
            now_serifu = 15;
        }
        sub_sub_step_up();
        break;

    case 3:
        serifu_run();
        break;

    case 4:
        didx = master_cw_rank;
        if ((u32)didx >= 3U) {
            now_serifu = 22;
        } else {
            now_serifu = 27;
        }
        sub_sub_step_up();
        break;

    case 5:
        serifu_run();
        break;

    case 6:
        fade_out_step();
        flags = (u32)(u8)(cal_cw_bright - 4);
        cal_cw_bright = (u8)flags;
        if (flags < 4U) {
            cal_cw_bright = 4;
        }
        break;

    case 7:
        cal_cw_disp = 0;
        cal_cw_mode = 0;
        sub_step_up();
        flags = bukatu_pt6_w;
        rot = ((u32)(flags << 5)) >> 30;
        if (rot < 3) {
            sub_step_up();
        }
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
}
