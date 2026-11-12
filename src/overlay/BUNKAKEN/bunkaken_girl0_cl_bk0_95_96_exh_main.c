/* @SymbolStart: 0x8014A9E8 */
/* @RodataBase: 0x80155278 */
/* @SymbolSize: 1440 */
/* Address: 0x8014A9F0 */
/* Size: 1432 bytes (358 insns) */
/* @PostProc: patch_insn_reg_field 0x238:rd:t9:t0 0x23c:rs:t9:t0 0x254:rt:t0:t1 0x258:rs:t0:t1 0x258:rt:t0:t2 0x260:rs:t0:t2 0x260:rt:t1:t3 0x264:rt:t1:t3 0x26c:rt:t2:t4 0x274:rt:t2:t4 0x2a0:rt:t3:t5 0x2a4:rs:t3:t5 0x2a4:rt:t3:t5 0x2ac:rs:t3:t5 0x2f4:rt:t7:t9 0x300:rt:t0:t2 0x30c:rt:t4:t6 0x310:rs:t4:t6 0x310:rt:t4:t6 0x318:rs:t4:t6 0x31c:rt:t6:t8 0x320:rt:t5:t7 0x32c:rt:t5:t7 0x338:rt:t6:t8 0x33c:rs:t7:t9 0x33c:rt:t7:t9 0x344:rs:t7:t9 0x348:rt:t9:t1 0x34c:rt:t8:t0 0x358:rt:t8:t0 0x364:rt:t9:t1 0x368:rs:t0:t2 0x368:rt:t0:t2 0x370:rs:t0:t2 0x374:rt:t2:t4 0x378:rt:t1:t3 0x384:rt:t1:t3 0x38c:rt:t2:t4 0x3c0:rt:t3:t5 0x3c4:rs:t3:t5 0x3c4:rt:t3:t5 0x3c8:rt:t4:t6 0x3cc:rs:t3:t5 0x3e8:rt:t4:t6 0x414:rt:t5:t7 0x418:rs:t5:t7 0x418:rt:t5:t7 0x420:rs:t5:t7 0x454:rt:t9:t1 0x460:rt:t2:t4 0x46c:rt:t6:t8 0x470:rs:t6:t8 0x470:rt:t6:t8 0x478:rs:t6:t8 0x47c:rt:t8:t0 0x480:rt:t7:t9 0x48c:rt:t7:t9 0x498:rt:t8:t0 0x49c:rs:t9:t1 0x49c:rt:t9:t1 0x4a4:rs:t9:t1 0x4a8:rt:t1:t3 0x4ac:rt:t0:t2 0x4b8:rt:t0:t2 0x4c4:rt:t1:t3 0x4c8:rs:t2:t4 0x4c8:rt:t2:t4 0x4d0:rs:t2:t4 0x4d4:rt:t4:t6 0x4d8:rt:t3:t5 0x4e4:rt:t3:t5 0x4ec:rt:t4:t6 0x520:rt:t5:t7 0x524:rs:t5:t7 0x524:rt:t5:t7 0x52c:rs:t5:t7 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32  dec_bg_init(s32);
extern void bustup_cd_read(s32);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32  get_g_zyotai_low(s32);
extern void bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);
extern void bunkaken_param_set2(void);

extern s32 bunka_girl2;
extern s16 now_serifu;
extern u8  now_year;
extern u8  now_girl;
extern s32 girl0_flag;
extern u8  girl0_flag_b;
extern u8 now_sub_sub_step;

void bunkaken_girl0_cl_bk0_95_96_exh_main(void)
{
    s32 idx = now_sub_sub_step;
    s32 v0;
    s32 ret;

    switch (idx) {
    case 0:
        value_cache_find_or_load(16618, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkaken_basyo_init3();
            ret = dec_bg_init(1);
            indexed_byte_lsb_set(1, ret);
            bustup_cd_read(21578);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 0;
            zyotai_sprite_set_chk();
            cd_read_start2(23, 0x801E8000, 18923);
        }
        break;
    case 5:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4 || bunka_girl2 == 0) {
            sub_sub_step_up();
        } else {
            fade_up_main();
        }
        break;
    case 6:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(0);
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
            if (bunka_girl2 == 0 || get_g_zyotai_low(0) == 4) {
                if (now_year == 95) {
                    now_serifu = 13;
                } else {
                    now_serifu = 22;
                }
            }
            bunkaken_param_set2();
        }
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        {
            s32 t8 = girl0_flag << 29;
            if (t8 >= 0 && get_g_zyotai_low(0) != 4) {
                girl0_flag_b = girl0_flag_b | 4;
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
        if (bunka_girl2 == 0 || get_g_zyotai_low(0) == 4) {
            goto_sub_sub_step(12);
        } else {
            v0 = get_g_zyotai_low(0);
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
        }
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4 || bunka_girl2 == 0) {
            fade_out_step();
        } else {
            now_serifu = 14;
            fade_out();
        }
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4 || bunka_girl2 == 0) {
            fade_in_step();
        } else {
            v0 = get_g_zyotai_low(0);
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
            fade_in();
        }
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4 || bunka_girl2 == 0) {
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
