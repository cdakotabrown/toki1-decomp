/* @SymbolStart: 0x8013C0AC */
/* @RodataBase: 0x80153F00 */
/* @SymbolSize: 1416 */
/* @PostProc: patch_insn_reg_field 0x250:rt:v0:t0 0x264:rs:v0:t0 0x264:rt:v0:t0 0x268:rt:t1:t3 0x26c:rd:t0:t2 0x26c:rt:v0:t0 0x270:rs:t0:t2 0x278:rs:t1:t3 0x278:rt:t1:t4 0x27c:rt:t3:t6 0x280:rs:t1:t4 0x280:rt:t2:t5 0x284:rt:t2:t5 0x290:rt:t3:t6 0x2d8:rt:t7:t0 0x2e4:rt:t0:t3 0x2f0:rt:t4:t7 0x2f4:rs:t4:t7 0x2f4:rt:t4:t7 0x2fc:rs:t4:t7 0x300:rt:t6:t9 0x304:rt:t5:t8 0x310:rt:t5:t8 0x31c:rt:t6:t9 0x320:rs:t7:t0 0x320:rt:t7:t0 0x328:rs:t7:t0 0x32c:rt:t9:t2 0x330:rt:t8:t1 0x33c:rt:t8:t1 0x348:rt:t9:t2 0x34c:rs:t0:t3 0x34c:rt:t0:t3 0x354:rs:t0:t3 0x358:rt:t2:t5 0x35c:rt:t1:t4 0x368:rt:t1:t4 0x370:rt:t2:t5 0x3a4:rt:t3:t6 0x3a8:rs:t3:t6 0x3a8:rt:t3:t6 0x3b0:rs:t3:t6 0x3d0:rt:t4:t7 0x3dc:rt:t4:t7 0x400:rt:t5:t8 0x404:rs:t5:t8 0x404:rt:t5:t8 0x40c:rs:t5:t8 0x448:rt:t9:t2 0x454:rt:t2:t5 0x460:rt:t6:t9 0x464:rs:t6:t9 0x464:rt:t6:t9 0x46c:rs:t6:t9 0x470:rt:t8:t1 0x474:rt:t7:t0 0x480:rt:t7:t0 0x48c:rt:t8:t1 0x490:rs:t9:t2 0x490:rt:t9:t2 0x498:rs:t9:t2 0x49c:rt:t1:t4 0x4a0:rt:t0:t3 0x4ac:rt:t0:t3 0x4b8:rt:t1:t4 0x4bc:rs:t2:t5 0x4bc:rt:t2:t5 0x4c4:rs:t2:t5 0x4c8:rt:t4:t7 0x4cc:rt:t3:t6 0x4d8:rt:t3:t6 0x4e4:rt:t4:t7 0x508:rt:t5:t8 0x50c:rs:t5:t8 0x50c:rt:t5:t8 0x514:rs:t5:t8 */
/* Address: 0x8013C0B4 */
/* Size: 1408 bytes (352 insns) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 goto_sub_sub_step(s32);
extern s32 get_CD_State2(void);
extern s32 cd_read_start2(s32, s32, s32);
extern s32 game_reset(void);
extern s32 indexed_byte_lsb_set(s32, s32);
extern s32 value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern s32 bustup_cd_read(s32);
extern s32 bustup_ptr_init(void);
extern s32 fade_up_main(void);
extern s32 fade_down_main(void);
extern s32 fade_in_step(void);
extern s32 fade_in(void);
extern s32 fade_out_step(void);
extern s32 fade_out(void);
extern s32 zyotai_sprite_set_chk(void);
extern s32 serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bunkaken_girl1_cl_bank_init(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern u8  bunkaken_sub_flag;
extern u8  now_girl;
extern s32 bunka_girl2;
extern s16 now_serifu;
extern u8  now_year;
extern u8  girl1_flag;
extern u8 now_sub_sub_step;

void bunkaken_main2d(void) {
    s32 idx = now_sub_sub_step;
    s32 v0;
    s32 t;

    switch ((u32)idx) {
    case 0:
        value_cache_find_or_load(16618, 1);
        bunkaken_sub_flag = 0;
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkaken_basyo_init3();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            bustup_cd_read(23873);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 1;
            zyotai_sprite_set_chk();
            cd_read_start2(7, 0x801E8000, 19126);
        }
        break;
    case 5:
        v0 = get_g_zyotai_low(1);
        if (v0 == 4 || bunka_girl2 == 1) {
            sub_sub_step_up();
        } else {
            fade_up_main();
        }
        break;
    case 6:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 1;
                break;
            case 2:
                now_serifu = 2;
                break;
            case 3:
                now_serifu = 3;
                break;
            }
            v0 = get_g_zyotai_low(1);
            if (v0 == 4 || bunka_girl2 == 1) {
                if (now_year == 95) {
                    now_serifu = 13;
                } else {
                    now_serifu = 22;
                }
            }
            bunkaken_girl1_cl_bank_init();
        }
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        v0 = get_g_zyotai_low(1);
        if (v0 == 4 || bunka_girl2 == 1) {
            goto_sub_sub_step(12);
        } else {
            t = *(s32 *)&girl1_flag;
            if ((t << 29) >= 0) {
                girl1_flag = girl1_flag | 4;
                now_serifu = 4;
                sub_sub_step_up();
            } else {
                goto_sub_sub_step(10);
            }
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(1);
        switch (v0) {
        case 0:
        case 1:
            if (now_year == 95) {
                now_serifu = 7;
            } else {
                now_serifu = 16;
            }
            break;
        case 2:
            if (now_year == 95) {
                now_serifu = 9;
            } else {
                now_serifu = 18;
            }
            break;
        case 3:
            if (now_year == 95) {
                now_serifu = 11;
            } else {
                now_serifu = 20;
            }
            break;
        }
        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        v0 = get_g_zyotai_low(1);
        if (v0 == 4 || bunka_girl2 == 1) {
            fade_out_step();
        } else {
            fade_out();
            now_serifu = 14;
        }
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        v0 = get_g_zyotai_low(1);
        if (v0 == 4 || bunka_girl2 == 1) {
            fade_in_step();
        } else {
            fade_in();
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                if (now_year == 95) {
                    now_serifu = 8;
                } else {
                    now_serifu = 17;
                }
                break;
            case 2:
                if (now_year == 95) {
                    now_serifu = 10;
                } else {
                    now_serifu = 19;
                }
                break;
            case 3:
                if (now_year == 95) {
                    now_serifu = 12;
                } else {
                    now_serifu = 21;
                }
                break;
            }
        }
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        v0 = get_g_zyotai_low(1);
        if (v0 == 4 || bunka_girl2 == 1) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 17:
        fade_out_step();
        break;
    case 18:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 19:
        serifu_run();
        break;
    case 20:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
