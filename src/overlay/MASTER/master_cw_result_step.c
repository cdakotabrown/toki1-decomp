/* @SymbolStart: 0x801344E0 */
/* @SymbolSize: 2912 */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic0 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=60:t1,72:t5 */
/* @RodataBase: 0x8013BD2C */
/* @PostProc: remap_stack_slots --map '44:52,36:48,32:52' */
/* @PostProc: swap_instructions --offset1 0xe4 --offset2 0xec */
/* @PostProc: swap_instructions --offset1 0x148 --offset2 0x150 */
/* @PostProc: swap_instructions --offset1 0x1ac --offset2 0x1b4 */
/* @PostProc: swap_instructions --offset1 0x614 --offset2 0x618 */
/* @PostProc: swap_instructions --offset1 0x7c0 --offset2 0x7d4 */
/* @PostProc: swap_instructions --offset1 0x7c4 --offset2 0x7cc */
/* @PostProc: swap_instructions --offset1 0xad4 --offset2 0xadc */
/* @PostProc: swap_instructions --offset1 0xae0 --offset2 0xae4 */
/* @PostProc: swap_instructions --offset1 0xae4 --offset2 0xae8 */
/* @PostProc: swap_instructions --offset1 0xae8 --offset2 0xaec */
/* @PostProc: swap_instructions --offset1 0xaec --offset2 0xaf4 */
/* @PostProc: patch_insn_reg_field 0xa8:rt:v1:v0 0xac:rt:v0:v1 0xc0:rt:t1:t3 0xc4:rt:t3:t5 0xc8:rt:t5:t7 0xcc:rt:t7:t1 0xd4:rs:t1:t3 0xd4:rt:t2:t4 0xd8:rs:t3:t5 0xd8:rt:t4:t6 0xdc:rs:t5:t7 0xdc:rt:t6:t8 0xe0:rs:t7:t1 0xe0:rt:t8:t2 0xe4:rt:t6:t8 0xe8:rt:t4:t6 0xec:rt:t2:t4 0xf4:rt:t8:t2 0x10c:rt:v1:v0 0x110:rt:v0:v1 0x124:rt:t1:t3 0x128:rt:t3:t5 0x12c:rt:t5:t7 0x130:rt:t7:t1 0x138:rs:t1:t3 0x138:rt:t2:t4 0x13c:rs:t3:t5 0x13c:rt:t4:t6 0x140:rs:t5:t7 0x140:rt:t6:t8 0x144:rs:t7:t1 0x144:rt:t8:t2 0x148:rt:t6:t8 0x14c:rt:t4:t6 0x150:rt:t2:t4 0x158:rt:t8:t2 0x170:rt:v1:v0 0x174:rt:v0:v1 0x188:rt:t1:t3 0x18c:rt:t3:t5 0x190:rt:t5:t7 0x194:rt:t7:t1 0x19c:rs:t1:t3 0x19c:rt:t2:t4 0x1a0:rs:t3:t5 0x1a0:rt:t4:t6 0x1a4:rs:t5:t7 0x1a4:rt:t6:t8 0x1a8:rs:t7:t1 0x1a8:rt:t8:t2 0x1ac:rt:t6:t8 0x1b0:rt:t4:t6 0x1b4:rt:t2:t4 0x1bc:rt:t8:t2 0x2d4:rt:v0:v1 0x2d8:rs:v0:v1 0x2d8:rt:v0:v1 0x2e0:rs:v0:v1 0x2e0:rt:v0:v1 0x2e4:rs:v0:v1 0x5c8:rt:v1:a0 0x5cc:rt:a0:v1 0x648:rt:v0:v1 0x654:rt:v0:v1 0x758:rt:a0:v1 0x75c:rt:v1:v0 0x768:rs:a0:v0 0x768:rt:a0:v0 0x770:rt:t4:t3 0x774:rt:t2:t4 0x778:rt:t3:t2 0x77c:rs:t4:t3 0x77c:rt:t5:t7 0x780:rs:t2:t4 0x780:rt:t6:t5 0x784:rs:t3:t2 0x784:rt:t7:t6 0x788:rt:t8:t2 0x78c:rt:t0:t4 0x790:rt:t4:t3 0x794:rt:t2:t8 0x798:rt:t3:t0 0x79c:rt:t5:t7 0x7a0:rt:t6:t5 0x7a4:rt:t7:t6 0x7ac:rs:t8:t2 0x7ac:rt:t9:t6 0x7b0:rs:t0:t4 0x7b0:rt:t1:t5 0x7b4:rs:t4:t3 0x7b4:rt:t5:t7 0x7b8:rs:t2:t8 0x7b8:rt:t6:t9 0x7bc:rs:t3:t0 0x7bc:rt:t7:t1 0x7c0:rt:t7:t1 0x7c4:rt:t6:t9 0x7c8:rt:t5:t7 0x7cc:rt:t1:t5 0x7d0:rt:a0:v0 0x7d4:rt:t9:t6 0x820:rt:v0:v1 0x824:rs:v0:v1 0x824:rt:v0:v1 0x82c:rs:v0:v1 0x82c:rt:v0:v1 0x830:rs:v0:v1 0x94c:rt:v0:v1 0x950:rs:v0:v1 0x950:rt:v0:v1 0x958:rs:v0:v1 0x958:rt:v0:v1 0x95c:rs:v0:v1 0x9ec:rt:a0:v1 0x9f0:rt:v1:v0 0x9fc:rs:a0:v0 0x9fc:rt:a0:v0 0xa0c:rt:a0:v0 0xa98:rt:a0:v1 0xa9c:rt:v1:a0 0xab0:rt:t1:t5 0xab4:rt:t5:t6 0xab8:rt:t6:t7 0xabc:rt:t7:t1 0xac4:rd:t4:t2 0xac4:rs:t1:t5 0xac8:rd:t2:t3 0xac8:rs:t5:t6 0xacc:rd:t3:t8 0xacc:rs:t6:t7 0xad0:rd:t8:t4 0xad0:rs:t7:t1 0xad4:rt:t3:t8 0xad8:rt:t2:t3 0xadc:rt:t4:t2 0xaf4:rt:t8:t4 */
/* master_cw_result_step - MASTER overlay, Address: 0x801344E8, Size: 2904 bytes */
#include "types.h"

typedef struct {
    u8  f0;
    u8  f1;
    u8  f2;
    u8  f3;
    u8  f4;
    u8  f5;
    u8  f6;
    u8  f7;
    s16 f8;
    s16 f10;
    s32 f12;
    s32 f16;
    s16 f20;
    s16 f22;
    s16 f24;
    u8  pad26[12];
    s16 f38;
    s16 f40;
    s16 f42;
    u8  pad44[8];
    s32 f52;
    s32 f56;
    u8  pad60[7];
    u8  f67;
} Blk;

extern u8  master_cw_rank;
extern s32 master_cw_no;
extern u8  master_yazirushi_cnt;
extern u8  day_flag;
extern u8  schedule_cursor;
extern s16 girl0_tokimeki;
extern u8  back_clear_b[];
extern u8  girl5_blk_end[];
extern u8  p_sei[];
extern s32 pad_trig;
extern u8  update_anim_state_tbl4;
extern u8  yazirushi_cw_disp;
extern Blk cw96_active[20];
extern Blk cw97_active[];
extern Blk cw100_active[];
extern u8  cw116_active[];
extern s32 master_result_desc_tbl[][2];
extern s32 master_result_pal_ptr;
extern s16 master_snow_xy[];
extern s16 master_snow_spd[];
extern u8  master_result4_str0[];
extern u8  master_result4_str1[];
extern u8  master_result4_str2[];
extern u8  master_result4_str3[];
extern u8  master_result4_str4[];
extern u8  master_result4_str5[];
extern u8  master_result4_str6[];

extern void prim_init(void);
extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void device_cmd_send(s32);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void tpage_buf_clear(void);
extern void class_clear_n(s32);
extern void set_kanji_string(s32, s32, s32, u8 *, s32);
extern void k_disp_start(s32);
extern void k_reset(s32);
extern s32  check_end_k(void);
extern void set_dec_bri(s32);
extern void hizuke_disp_switch(s32);
extern void hizuke_init(void);
extern void mes_window_switch(s32);
extern void schedule_cursor_set(void);
extern void return_schedule(s32);
extern void hizuke_advance(void);
extern void fade_out_step(void);
extern void shared_system_state_reset(void);
extern void common_disp(void);
extern s32  rand(void);
extern void master_result_res_set(void);
extern void master_snow(void);
extern u8 now_sub_sub_step;

void master_cw_result_step(void) {
    s32 idx;
    u8 cnt;
    u8 *rp;
    u8 *rpe;
    Blk *bp;
    Blk *bpe;
    s16 *pp;
    s16 *qp;

    switch (now_sub_sub_step) {
    case 0:
        if (master_cw_rank >= 3U) {
            idx = master_cw_no;
            day_flag = 0;
            idx += 1;
            master_cw_no = idx;
            switch (idx) {
            case 1:
            case 2:
                break;
            case 3:
                girl0_tokimeki += 15;
                rpe = girl5_blk_end;
                rp = back_clear_b;
                do {
                    *(s16 *)(rp + 502) += 15;
                    *(s16 *)(rp + 558) += 15;
                    *(s16 *)(rp + 614) += 15;
                    *(s16 *)(rp + 446) += 15;
                    rp += 224;
                } while (rp != rpe);
                break;
            case 4:
                girl0_tokimeki += 3;
                rpe = girl5_blk_end;
                rp = back_clear_b;
                do {
                    *(s16 *)(rp + 502) += 3;
                    *(s16 *)(rp + 558) += 3;
                    *(s16 *)(rp + 614) += 3;
                    *(s16 *)(rp + 446) += 3;
                    rp += 224;
                } while (rp != rpe);
                break;
            case 5:
                girl0_tokimeki += 4;
                rpe = girl5_blk_end;
                rp = back_clear_b;
                do {
                    *(s16 *)(rp + 502) += 4;
                    *(s16 *)(rp + 558) += 4;
                    *(s16 *)(rp + 614) += 4;
                    *(s16 *)(rp + 446) += 4;
                    rp += 224;
                } while (rp != rpe);
                break;
            }
            hizuke_advance();
            schedule_cursor += 1;
            hizuke_init();
            device_cmd_send(512);
            sub_step_up();
        } else {
            cd_read_start2(110, 0x801B4400, 0xBBB3);
            tpage_buf_clear();
            update_anim_state_tbl4 = 10;
            master_yazirushi_cnt = 0;
            sub_sub_step_up();
        }
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        k_reset(1);
        set_kanji_string(-128, 50, 0, p_sei, 0);
        k_disp_start(2);
        set_kanji_string(-86, 50, 0, master_result4_str0, 0);
        set_kanji_string(-86, 66, 0, master_result4_str1, 0);
        set_kanji_string(-86, 82, 0, master_result4_str2, 0);
        sub_sub_step_up();
        break;
    case 3:
        cnt = master_yazirushi_cnt + 1;
        master_yazirushi_cnt = cnt;
        if (master_yazirushi_cnt >= 16U && check_end_k() != 0) {
            yazirushi_cw_disp ^= 0x80;
            master_yazirushi_cnt = 0;
        }
        if (pad_trig & 0xF0) {
            k_reset(1);
            yazirushi_cw_disp &= 0x7F;
            sub_sub_step_up();
        }
        break;
    case 4:
        if (get_CD_State2() & 1) {
            master_result_res_set();
            palette_upload_enqueue(master_result_pal_ptr, 16, 1, 0, 0);
            palette_upload_enqueue(master_result_pal_ptr, 17, 1, 0, 1);
            class_clear_n(96);
            cw96_active[0].f1 = 11;
            cw96_active[0].f2 = 0;
            cw96_active[0].f56 = 0x01000000;
            cw96_active[0].f3 = 4;
            cw96_active[0].f12 = master_result_desc_tbl[1][0];
            cw96_active[0].f16 = master_result_desc_tbl[2][0];
            cw96_active[0].f52 = master_result_desc_tbl[0][0];
            cw96_active[0].f20 = (*(s16 *)&master_result_desc_tbl[3][0]);
            cw96_active[0].f22 = 0;
            cw96_active[0].f24 = 0;
            cw96_active[0].f6 = 1;
            cw96_active[0].f67 = 0;
            cw96_active[0].f38 = -160;
            cw96_active[0].f42 = -120;
            cw96_active[0].f7 = 0;
            cw96_active[0].f5 = 128;
            cw96_active[0].f4 = 0;
            cw96_active[1] = cw96_active[0];
            cw96_active[1].f2 = 64;
            cw96_active[1].f12 = master_result_desc_tbl[1][1];
            cw96_active[1].f16 = master_result_desc_tbl[2][1];
            cw96_active[1].f52 = master_result_desc_tbl[0][1];
            cw96_active[1].f20 = (*(s16 *)&master_result_desc_tbl[3][1]);
            cw96_active[1].f38 = rand() % 320 - 160;
            cw96_active[1].f42 = 60;
            bp = &cw96_active[2];
            do {
                *bp = cw96_active[1];
                bp->f24 = rand() % 10;
                bp++;
            } while (bp < &cw96_active[20]);
            pp = master_snow_xy;
            qp = master_snow_spd;
            do {
                *qp = rand() % 5 + 1;
                pp[0] = rand() % 320 - 160;
                pp[1] = 60;
                qp += 1;
                pp += 2;
            } while (pp != master_snow_spd);
            hizuke_disp_switch(0);
            mes_window_switch(0);
            master_yazirushi_cnt = 15;
            sub_sub_step_up();
        }
        break;
    case 5:
        set_kanji_string(-128, 50, 0, p_sei, 0);
        k_disp_start(2);
        set_kanji_string(-86, 50, 0, master_result4_str3, 0);
        device_cmd_send(515);
        sub_sub_step_up();
        break;
    case 6:
        master_snow();
        cw96_active[0].f3 |= 0x80;
        cw96_active[0].f7 += 8;
        cw96_active[1].f3 |= 0x80;
        cw96_active[1].f7 += 8;
        cw96_active[2].f3 |= 0x80;
        cw96_active[2].f7 += 8;
        cw96_active[3].f3 |= 0x80;
        cw96_active[3].f7 += 8;
        bpe = (Blk *)cw116_active;
        bp = cw100_active;
        do {
            bp[1].f3 |= 0x80;
            bp[0].f3 |= 0x80;
            bp[0].f7 += 8;
            bp[3].f3 |= 0x80;
            bp[2].f7 += 8;
            bp[3].f7 += 8;
            bp[1].f7 += 8;
            bp[2].f3 |= 0x80;
            bp += 4;
        } while (bp != bpe);
        cw96_active[0].f2 = 5;
        if (cw96_active[0].f7 >= 128U) {
            cw96_active[0].f7 = 128;
            cw96_active[1].f7 = 128;
            sub_sub_step_up();
        }
        break;
    case 7:
        master_snow();
        cnt = master_yazirushi_cnt + 1;
        master_yazirushi_cnt = cnt;
        if (master_yazirushi_cnt >= 16U && check_end_k() != 0) {
            yazirushi_cw_disp ^= 0x80;
            master_yazirushi_cnt = 0;
        }
        if (pad_trig & 0xF0) {
            k_reset(1);
            yazirushi_cw_disp &= 0x7F;
            cw96_active[0].f22 = 2;
            cw96_active[0].f24 = 0;
            cw96_active[0].f8 = 0;
            cw96_active[0].f2 = 5;
            sub_sub_step_up();
        }
        break;
    case 8:
        master_snow();
        k_reset(1);
        set_kanji_string(-128, 50, 2, master_result4_str4, 0);
        set_kanji_string(-128, 66, 1, master_result4_str5, 0);
        set_kanji_string(-128, 82, 0, master_result4_str6, 0);
        master_yazirushi_cnt = 0;
        sub_sub_step_up();

    case 9:
        master_snow();
        cnt = master_yazirushi_cnt + 1;
        master_yazirushi_cnt = cnt;
        if (master_yazirushi_cnt >= 16U && check_end_k() != 0) {
            yazirushi_cw_disp ^= 0x80;
            master_yazirushi_cnt = 0;
        }
        if (pad_trig & 0xF0) {
            k_reset(1);
            yazirushi_cw_disp &= 0x7F;
            sub_sub_step_up();
        }
        break;
    case 10:
        master_snow();
        cw96_active[0].f7 -= 8;
        bpe = &cw96_active[20];
        bp = cw97_active;
        do {
            bp->f7 -= 8;
            bp++;
        } while (bp < bpe);
        if (cw96_active[0].f7 < 9U) {
            cw96_active[0].f7 = 0;
            cw96_active[0].f3 &= ~0x80;
            cw96_active[1].f7 = 0;
            cw96_active[1].f3 &= ~0x80;
            cw96_active[2].f7 = 0;
            cw96_active[2].f3 &= ~0x80;
            cw96_active[3].f7 = 0;
            cw96_active[3].f3 &= ~0x80;
            bpe = (Blk *)cw116_active;
            bp = cw100_active;
            do {
                bp[1].f3 &= ~0x80;
                bp[2].f3 &= ~0x80;
                bp[3].f3 &= ~0x80;
                bp[0].f3 &= ~0x80;
                bp[1].f7 = 0;
                bp[2].f7 = 0;
                bp[3].f7 = 0;
                bp[0].f7 = 0;
                bp += 4;
            } while (bp != bpe);
            update_anim_state_tbl4 = 15;
            prim_init();
            sub_sub_step_up();
        }
        break;
    case 11:
        day_flag |= 0x10;
        shared_system_state_reset();
        set_dec_bri(0);
        schedule_cursor_set();
        return_schedule(1);
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
}
