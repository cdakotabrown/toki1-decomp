/* @PostProc: patch_stack_complete --old 72 --new 64 */
/* @PostProc: remap_stack_slots --map '52:48,56:52' */
/* @PostProc: swap_instructions --offset1 0x638 --offset2 0x63c */
/* @PostProc: swap_instructions --offset1 0x8ec --offset2 0x8f0 */
/* @PostProc: swap_instructions --offset1 0x998 --offset2 0x99c */
/* @PostProc: swap_instructions --offset1 0x690 --offset2 0x6a4 */
/* @PostProc: swap_instructions --offset1 0x694 --offset2 0x69c */
/* @PostProc: swap_instructions --offset1 0x6d0 --offset2 0x6dc */
/* @PostProc: swap_instructions --offset1 0x9c4 --offset2 0x9cc */
/* @PostProc: swap_instructions --offset1 0x9d0 --offset2 0x9d4 */
/* @PostProc: swap_instructions --offset1 0x9d4 --offset2 0x9d8 */
/* @PostProc: swap_instructions --offset1 0x9d8 --offset2 0x9dc */
/* @PostProc: swap_instructions --offset1 0x9dc --offset2 0x9e4 */
/* @PostProc: patch_insn_reg_field 0x62c:rt:a0:v0 0x638:rs:a0:v0 0x638:rt:a0:v0 0x640:rt:t1:t0 0x644:rt:t9:t1 0x648:rt:t0:t9 0x64c:rs:t1:t0 0x64c:rt:t2:t4 0x650:rs:t9:t1 0x650:rt:t3:t2 0x654:rs:t0:t9 0x654:rt:t4:t3 0x658:rt:t5:t9 0x65c:rt:t7:t1 0x660:rt:t1:t0 0x664:rt:t9:t5 0x668:rt:t0:t7 0x66c:rt:t2:t4 0x670:rt:t3:t2 0x674:rt:t4:t3 0x67c:rs:t5:t9 0x67c:rt:t6:t3 0x680:rs:t7:t1 0x680:rt:t8:t2 0x684:rs:t1:t0 0x684:rt:t2:t4 0x688:rs:t9:t5 0x688:rt:t3:t6 0x68c:rs:t0:t7 0x68c:rt:t4:t8 0x690:rt:t4:t8 0x694:rt:t3:t6 0x698:rt:t2:t4 0x69c:rt:t8:t2 0x6a0:rt:a0:v0 0x6a4:rt:t6:t3 0x6d0:rt:t8:t7 0x6dc:rt:t7:t8 0x9a0:rt:t8:t2 0x9a4:rt:t2:t3 0x9a8:rt:t3:t4 0x9ac:rt:t4:t8 0x9b4:rd:t1:t9 0x9b4:rs:t8:t2 0x9b8:rd:t9:t0 0x9b8:rs:t2:t3 0x9bc:rd:t0:t5 0x9bc:rs:t3:t4 0x9c0:rd:t5:t1 0x9c0:rs:t4:t8 0x9c4:rt:t0:t5 0x9c8:rt:t9:t0 0x9cc:rt:t1:t9 0x9e4:rt:t5:t1 */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic1 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=27:t8,39:t2 IDO52_FORCE_WEB_COLOR=205:v1,6:v1 IDO52_EXCLUDE_REGS_WEB=150:s0+s1+s2+s3+s4+s5+s6+s7+s8 */
/* @SymbolStart: 0x801363F4 */
/* @SymbolSize: 2608 */
/* @RodataBase: 0x8013BE64 */
/* master_result_rank_snow_main - MASTER overlay, Address: 0x801363F4 (absorbed head) / */
/* 0x801363FC (prologue), Size: 2608 bytes (652 insns). 12-case step */
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
extern u8  master_yazirushi_cnt;
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
typedef struct { s16 x; s16 y; } XY;
extern XY master_snow_xy[];
extern s16 master_snow_spd[];
extern u8  master_result19_str0[];
extern u8  master_result19_str1[];
extern u8  master_result19_str2[];
extern u8  master_result19_str3[];
extern u8  master_result19_str4[];
extern u8  master_result19_str5[];
extern u8  master_result19_str6[];
extern u8  master_result19_str7[];
extern u8  master_result19_str8[];
extern u8  master_result19_str9[];

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
extern void hizuke_disp_switch(s32);
extern void mes_window_switch(s32);
extern void common_disp(void);
extern s32  rand(void);
extern void master_result_res_set(void);
extern void master_snow(void);
extern u8 now_sub_sub_step;

void master_result_rank_snow_main(void) {
    u8 cnt;
    Blk *bp;
    s32 i;

    switch ((u32)now_sub_sub_step) {
    case 0:
        device_cmd_send(193);
        cd_read_start2(110, 0x801B4400, 0xBBB3);
        tpage_buf_clear();
        update_anim_state_tbl4 = 10;
        master_yazirushi_cnt = 0;
        sub_sub_step_up();
        break;
    case 1:
        k_reset(1);
        set_kanji_string(-128, 50, 0, p_sei, 0);
        k_disp_start(2);
        if (master_cw_rank >= 3U) {
            set_kanji_string(-86, 50, 0, master_result19_str0, 0);
            set_kanji_string(-86, 66, 0, master_result19_str1, 0);
            set_kanji_string(-86, 82, 0, master_result19_str2, 0);
        } else {
            set_kanji_string(-86, 50, 0, master_result19_str3, 0);
            set_kanji_string(-86, 66, 0, master_result19_str4, 0);
            set_kanji_string(-86, 82, 0, master_result19_str5, 0);
        }
        sub_sub_step_up();
        break;
    case 2:
        cnt = master_yazirushi_cnt + 1;
        master_yazirushi_cnt = cnt;
        if (master_yazirushi_cnt >= 16U && check_end_k() != 0) {
            yazirushi_cw_disp ^= 0x80;
            master_yazirushi_cnt = 0;
        }
        if ((pad_trig & 0xF0) && check_end_k() != 0) {
            k_reset(1);
            yazirushi_cw_disp &= 0x7F;
            sub_sub_step_up();
        }
        break;
    case 3:
        sub_sub_step_up();
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
            i = 2;
            do {
                cw96_active[i] = cw96_active[1];
                cw96_active[i].f24 = rand() % 10;
                i++;
            } while (i < 20);
            for (i = 0; i < 10; i++) {
                master_snow_spd[i] = rand() % 5 + 1;
                master_snow_xy[i].x = rand() % 320 - 160;
                master_snow_xy[i].y = 60;
            }
            hizuke_disp_switch(0);
            mes_window_switch(0);
            master_yazirushi_cnt = 15;
            sub_sub_step_up();
        }
        break;
    case 5:
        set_kanji_string(-128, 50, 0, p_sei, 0);
        k_disp_start(2);
        set_kanji_string(-86, 50, 0, master_result19_str6, 0);
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
        } while (bp != (Blk *)cw116_active);
        cw96_active[0].f2 = 5;
        if (cw96_active[0].f7 >= 128U) {
            cw96_active[1].f7 = 128;
            cw96_active[0].f7 = 128;
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
        if ((pad_trig & 0xF0) && check_end_k() != 0) {
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
        set_kanji_string(-128, 50, 2, master_result19_str7, 0);
        set_kanji_string(-128, 66, 1, master_result19_str8, 0);
        set_kanji_string(-128, 82, 0, master_result19_str9, 0);
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
        if ((pad_trig & 0xF0) && check_end_k() != 0) {
            k_reset(1);
            yazirushi_cw_disp &= 0x7F;
            sub_sub_step_up();
        }
        break;
    case 10:
        master_snow();
        cw96_active[0].f7 -= 8;

        bp = cw97_active;
        do {
            bp->f7 -= 8;
            bp++;
        } while (bp < &cw96_active[20]);
        if (cw96_active[0].f7 < 9U) {
            cw96_active[0].f7 = 0;
            cw96_active[0].f3 &= ~0x80;
            cw96_active[1].f7 = 0;
            cw96_active[1].f3 &= ~0x80;
            cw96_active[2].f7 = 0;
            cw96_active[2].f3 &= ~0x80;
            cw96_active[3].f7 = 0;
            cw96_active[3].f3 &= ~0x80;

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
            } while (bp != (Blk *)cw116_active);
            update_anim_state_tbl4 = 15;
            prim_init();
            device_cmd_send(193);
            sub_sub_step_up();
        }
        break;
    case 11:
        sub_step_up();
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
}
