/* @CompileFlags: -Wc,-pic0 */
/* @DataBase: 0x801342B4 */
/* @SymbolSize: 1688 */
/* @RodataBase: 0x80134118 */
/* @PostProc: swap_instructions --offset1 0x11c --offset2 0x120 */
/* @PostProc: patch_insn_reg_field 0xec:rt:t4:t7 0xf0:rs:t4:t7 0xf0:rt:t4:t7 0xf4:rd:t7:t6 0xf8:rd:t6:t4 0xf8:rt:t7:t6 0xfc:rs:t6:t7 0x114:rs:t5:v0 0x114:rt:v0:t5 0x11c:rt:t9:t1 0x120:rs:t2:t1 0x120:rt:t2:t1 0x124:rs:t9:t2 0x124:rt:t9:t2 0x128:rs:t2:t1 0x128:rt:t1:t9 0x12c:rs:t1:t2 0x158:rd:t7:t6 0x15c:rd:t6:t7 0x15c:rt:t7:t6 0x164:rs:t6:t7 0x1d4:rt:t2:t1 0x1d8:rs:t2:t1 0x1d8:rt:t2:t1 0x1dc:rt:t1:t2 0x1e0:rs:t2:t1 0x1ec:rs:t1:t2 0x1ec:rt:t1:t2 0x1f4:rs:t1:t2 0x264:rt:t2:t9 0x28c:rt:t7:t6 0x294:rd:t6:t7 0x294:rt:t7:t6 0x298:rt:t6:t7 0x2c8:rt:t2:t1 0x2d4:rs:t2:t1 0x2d4:rt:t2:t1 0x2dc:rd:t1:t2 0x2dc:rt:t2:t1 0x2e0:rs:t1:t2 0x304:rd:t7:t6 0x308:rd:t6:t7 0x308:rt:t7:t6 0x30c:rs:t6:t7 0x39c:rt:v0:t3 0x3a0:rt:v1:t5 0x3a4:rt:a0:t1 0x3a8:rt:a1:t2 0x3ac:rs:v0:t5 0x3ac:rt:v0:t5 0x3b0:rs:v1:t3 0x3b0:rt:v1:t3 0x3b4:rs:a0:t2 0x3b4:rt:a0:t2 0x3b8:rs:a1:t1 0x3b8:rt:a1:t1 0x3bc:rd:t3:v0 0x3bc:rs:v1:t3 0x3bc:rt:v0:t5 0x3c0:rd:t5:v1 0x3c0:rs:a1:t1 0x3c0:rt:a0:t2 0x3c4:rs:t3:v0 0x3c4:rt:t5:v1 0x3cc:rt:t2:t9 0x3e0:rt:t2:t9 0x3f4:rd:t1:t8 0x3f8:rs:t1:t8 0x404:rt:t9:t0 0x408:rs:t9:t0 0x408:rt:t9:t0 0x40c:rt:t0:t7 0x410:rd:t8:t6 0x410:rt:t9:t0 0x414:rd:t8:t6 0x414:rs:t8:t6 0x414:rt:t9:t0 0x418:rd:t8:t6 0x418:rt:t8:t6 0x41c:rd:t0:t7 0x41c:rs:t0:t7 0x41c:rt:t8:t6 0x420:rs:t0:t7 0x420:rt:t0:t7 0x428:rd:t7:t4 0x428:rt:t0:t7 0x42c:rs:t1:t8 0x42c:rt:t7:t4 0x448:rt:t3:t1 0x45c:rt:t6:t3 0x460:rs:t6:t3 0x460:rt:t6:t3 0x468:rd:t4:t5 0x468:rt:t6:t3 0x46c:rt:t4:t5 0x484:rs:t3:t1 0x484:rt:t3:t1 0x48c:rd:t5:t2 0x48c:rt:t3:t1 0x490:rt:t5:t2 0x4b4:rt:t2:t9 0x4e0:rt:t1:t8 0x4e4:rs:t1:t8 0x4e8:rt:t9:t0 0x4ec:rs:t9:t0 0x4ec:rt:t9:t0 0x4f4:rd:t8:t6 0x4f4:rt:t9:t0 0x4f8:rs:t8:t6 0x4f8:rt:t0:t7 0x4fc:rs:t0:t7 0x504:rd:t0:t7 0x504:rt:t0:t7 0x50c:rt:t0:t7 0x510:rt:t0:t7 0x518:rs:t0:t7 0x57c:rt:t7:t4 0x580:rs:t7:t4 0x590:rt:t6:t3 0x598:rt:t6:t3 0x5b8:rt:v0:v1 0x5bc:rs:v0:v1 0x5bc:rt:v0:v1 0x5c0:rt:t3:t1 0x5c4:rd:t4:t5 0x5c4:rt:v0:v1 0x5c8:rt:t5:t2 0x5cc:rd:t5:t2 0x5cc:rs:t5:t2 0x5cc:rt:t4:t5 0x5d0:rd:t3:t1 0x5d0:rs:t3:t1 0x5d0:rt:t4:t5 0x5d8:rs:t3:t1 0x5d8:rt:t3:t1 0x5dc:rs:t5:t2 0x5dc:rt:t5:t2 0x5e0:rt:t4:t5 0x5e8:rd:v0:v1 0x5e8:rs:t4:t5 0x5f8:rt:t3:t1 0x600:rt:t5:t2 0x64c:rt:t2:t9 0x654:rt:t2:t9 */
/* rensyu_girl_pick_main - RENSYU overlay, Address: 0x80132390, Size: 1688 bytes */
#include "types.h"

extern s32 get_top_girl(void);
extern u32 get_g_zyotai_low(s32 a0);
extern void bustup_cd_read(s32 a0);
extern void goto_sub_sub_step(s32 a0);
extern void sub_sub_step_up(void);
extern void sub_step_up(void);
extern void goto_sub_step(s32 a0);
extern s32 get_CD_State2(void);
extern void game_reset(void);
extern void device_cmd_send(s32 a0);
extern void device_load_start(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void value_cache_find_or_load(s32 a0, s32 a1);
extern void indexed_byte_lsb_set(s32 a0, s32 a1);
extern s32 dec_bg_init(s32 a0);
extern void mes_window_switch(s32 a0);
extern void fade_in_step(void);
extern void serifu_run(void);
extern void common_disp(void);
extern void rensyu_bustup_read(void);

typedef struct {
    u32 w[13];
} Blk52;


extern u8 now_sub_sub_step;
extern u8 rensyu_girl_no;
extern u32 reserved_date;
extern u16 reserved_date_lo;
extern u16 reserved_date_hi;
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u32 girl11_flag;
extern u8 girl11_spot11_cnt;
extern u8 denwa_cnt;
extern u8 girl12_spot12_cnt;
extern u16 basyo_pack;
extern u8 screen_w[];
extern s32 girl_head_tbl[];
extern s32 girl_body_tbl[];
extern s32 girl_seq_tbl[];
extern s16 mes_base_y;
extern s16 now_serifu;
extern s16 now_serifu_in;

void rensyu_girl_pick_main(void)
{
    s32 pad[2];
    s32 sel;
    u32 w;
    u32 month;
    Blk52 arrB = { {
        0x541D, 0x5D14, 0x5201, 0x5882, 0x6692, 0x5F8A, 0x6200, 0x5AF8,
        0x4FE5, 0x64A3, 0x6908, 0x6B51, 0x57FB
    } };
    Blk52 arrA = { {
        0x544A, 0x5D41, 0x522E, 0x58AF, 0x66BF, 0x5FB7, 0x622D, 0x5B25,
        0x5012, 0x64D0, 0x6935, 0x6B7E, 0x5828
    } };
    u32 vloc;
    s32 s0;
    u32 r;
    u32 row;


    switch (now_sub_sub_step) {
    case 0:
        sel = get_top_girl();
        w = reserved_date;
        if (((w << 2) >> 31) == 1
            && now_year == ((w << 20) >> 25)
            && now_month == ((u32)reserved_date_lo >> 12)
            && now_day == (reserved_date_hi & 0x1F)) {
            rensyu_girl_no = 255;
            goto_sub_sub_step(6);
            break;
        }
        w = girl11_flag;
        month = now_month;
        if (((w << 20) >> 29) >= 3 && girl11_spot11_cnt == 0) {
            girl11_spot11_cnt = 1;
            rensyu_girl_no = 11;
            if (month >= 6 && month < 10) {
                bustup_cd_read(arrB.w[11]);
            } else {
                bustup_cd_read(arrA.w[11]);
            }
            sub_sub_step_up();
            break;
        }
        if ((u32)denwa_cnt >= 65 && girl12_spot12_cnt == 0) {
            girl12_spot12_cnt = 1;
            rensyu_girl_no = 12;
            if (month >= 6 && month < 10) {
                bustup_cd_read(arrB.w[12]);
            } else {
                bustup_cd_read(arrA.w[12]);
            }
            sub_sub_step_up();
            break;
        }
        r = get_g_zyotai_low(sel);
        if (r < 2) {
            vloc = (u32)basyo_pack >> 12;
            rensyu_girl_no = sel;
            for (s0 = 0; s0 != 11; s0++) {
                if (vloc == ((u32)screen_w[s0 * 56 + 458] >> 4)
                    && ((*(u32 *)&screen_w[s0 * 56 + 456] << 30) >> 31)
                       == 1) {
                    r = get_g_zyotai_low(s0);
                    if (r < 2) {
                        rensyu_girl_no = s0;
                    }
                    if (((u32)screen_w[458] >> 4) == 10
                        && ((u32)basyo_pack >> 12) == 10
                        && ((*(u32 *)&screen_w[1016] << 30) >> 31) == 1) {
                        if (get_g_zyotai_low(0) < 2
                            && get_g_zyotai_low(10) >= 2) {
                            rensyu_girl_no = 0;
                        } else if (get_g_zyotai_low(0) >= 2
                                   && get_g_zyotai_low(10) < 2) {
                            rensyu_girl_no = 10;
                        } else if (get_g_zyotai_low(0) < 2
                                   && get_g_zyotai_low(10) < 2) {
                            w = *(s16 *)&screen_w[450];
                            month = *(s16 *)&screen_w[446];
                            r = *(s16 *)&screen_w[1010];
                            row = *(s16 *)&screen_w[1006];
                            if ((s32)(month + w) >= (s32)(row + r)) {
                                rensyu_girl_no = 0;
                            } else {
                                rensyu_girl_no = 10;
                            }
                        }
                    }
                }
                vloc = (u32)basyo_pack >> 12;
            }
            row = rensyu_girl_no;
            if (vloc == ((u32)screen_w[row * 56 + 458] >> 4)) {
                rensyu_bustup_read();
            } else {
                month = now_month;
                if (month >= 6 && month < 10) {
                    bustup_cd_read(arrB.w[rensyu_girl_no]);
                } else {
                    bustup_cd_read(arrA.w[rensyu_girl_no]);
                }
            }
            sub_sub_step_up();
        } else {
            rensyu_girl_no = 255;
            goto_sub_sub_step(6);
        }
        break;

    case 1:
        sub_sub_step_up();
        break;

    case 2:
        if (get_CD_State2() & 1) {
            switch ((u32)basyo_pack >> 12) {
            case 6:
            case 7:
            case 8:
                value_cache_find_or_load(16535, 1);
                break;
            case 9:
                value_cache_find_or_load(16544, 1);
                break;
            case 10:
                value_cache_find_or_load(16553, 1);
                break;
            default:
                game_reset();
                break;
            }
            sub_sub_step_up();
        }
        break;

    case 3:
        if (get_CD_State2() & 1) {
            mes_window_switch(1);
            mes_base_y = 192;
            indexed_byte_lsb_set(1, dec_bg_init(1));
            device_cmd_send(193);
            row = rensyu_girl_no;
            device_load_start(1, 0xC051, 0xC02B, girl_head_tbl[row],
                          girl_body_tbl[row], girl_seq_tbl[row]);
            sub_sub_step_up();
        }
        break;

    case 4:
        fade_in_step();
        break;

    case 5:
        device_cmd_send(113);
        sub_step_up();
        break;

    case 6:
        sub_sub_step_up();
        break;

    case 7:
        now_serifu = 16;
        now_serifu_in = 0;
        sub_sub_step_up();

    case 8:
        serifu_run();
        break;

    case 9:
        goto_sub_step(3);
        break;

    default:
        game_reset();
        break;
    }

    common_disp();
}
