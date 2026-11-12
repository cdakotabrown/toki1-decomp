/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* @SymbolStart: 0x8014B7F8 */
/* Address: 0x8014B800 */
/* Size: 1472 bytes (368 insns) */
/* @SymbolSize: 1480 */
/* @RodataBase: 0x801553AC */
/* @PostProc: patch_insn_reg_field 0x20c:rt:t0:t1 0x210:rd:t9:t0 0x214:rs:t9:t0 0x22c:rt:t1:t2 0x230:rs:t1:t2 0x230:rt:t1:t3 0x234:rt:t0:t1 0x23c:rs:t1:t3 0x23c:rt:t2:t4 0x244:rt:t2:t4 0x278:rd:t3:t5 0x280:rt:t3:t5 0x284:rt:t3:t5 0x28c:rs:t3:t5 0x294:rt:t4:t6 0x2a0:rt:t4:t6 0x2a4:rt:t5:t7 0x2b0:rt:t5:t7 0x2b4:rt:t6:t8 0x2c0:rt:t6:t8 0x2cc:rt:t7:t9 0x2d0:rs:t7:t9 0x2d0:rt:t7:t9 0x2d8:rs:t7:t9 0x2e8:rt:t9:t1 0x2ec:rs:t9:t1 0x2ec:rt:t9:t1 0x2f0:rt:t8:t0 0x2f4:rs:t8:t0 0x2f4:rt:t8:t0 0x2f8:rd:t0:t2 0x2f8:rt:t9:t1 0x2fc:rd:t1:t3 0x2fc:rs:t8:t0 0x2fc:rt:t0:t2 0x300:rs:t1:t3 0x300:rt:t2:t4 0x30c:rt:t2:t4 0x330:rt:t3:t5 0x334:rs:t3:t5 0x334:rt:t3:t5 0x33c:rs:t3:t5 0x374:rt:t7:t9 0x378:rt:t4:t6 0x37c:rs:t4:t6 0x37c:rt:t4:t6 0x380:rt:t6:t8 0x384:rs:t4:t6 0x38c:rt:t5:t7 0x398:rt:t5:t7 0x39c:rt:t6:t8 0x3a0:rs:t7:t9 0x3a0:rt:t7:t9 0x3a4:rt:t9:t1 0x3a8:rs:t9:t1 0x3a8:rt:t9:t1 0x3b0:rt:t7:t9 0x3b8:rs:t9:t1 0x3b8:rt:t8:t0 0x3bc:rt:t8:t0 0x3e8:rt:t0:t2 0x3f4:rt:t0:t2 0x430:rt:t1:t3 0x434:rs:t1:t3 0x434:rt:t1:t3 0x43c:rs:t1:t3 0x46c:rt:t2:t4 0x470:rs:t2:t4 0x488:rd:t3:t5 0x490:rt:t3:t5 0x494:rt:t3:t5 0x49c:rs:t3:t5 0x4a4:rt:t4:t6 0x4b0:rt:t4:t6 0x4b4:rt:t5:t7 0x4c0:rt:t5:t7 0x4c4:rt:t6:t8 0x4d0:rt:t6:t8 0x4d4:rt:t7:t9 0x4dc:rt:t7:t9 0x4e0:rt:t9:t1 0x4e4:rs:t9:t1 0x4e4:rt:t9:t1 0x4e8:rt:t0:t2 0x4ec:rs:t9:t1 0x4f0:rt:t1:t3 0x4f4:rt:t8:t0 0x4fc:rt:t8:t0 0x500:rs:t1:t3 0x500:rt:t1:t3 0x504:rs:t0:t2 0x504:rt:t0:t2 0x508:rd:t2:t4 0x508:rt:t1:t3 0x50c:rd:t3:t5 0x50c:rs:t0:t2 0x50c:rt:t2:t4 0x510:rs:t3:t5 0x510:rt:t4:t6 0x51c:rt:t4:t6 0x548:rt:t5:t7 0x54c:rs:t5:t7 0x54c:rt:t5:t7 0x554:rs:t5:t7 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_step(s32);
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
extern void bustup_load_step(s32);
extern void bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);
extern void bunkaken_param_set2(void);

extern s32 bunka_girl2;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;
extern volatile s16 now_serifu;
extern u8  now_year;
extern u8  now_girl;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern s32 girl0_flag;
extern u8  girl0_flag_lo;

void bunkaken_girl0_cl_bk2_show_main(void)
{
    s32 idx = now_sub_sub_step;
    s32 v0;
    s32 t9;
    s32 ret;

    switch (idx) {
    case 0:
        value_cache_find_or_load(16636, 1);
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
            bustup_cd_read(21713);
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
        t9 = bunka_girl2;
        if (v0 == 4 || t9 == 0) {
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
            case 4:
                now_serifu = 50;
            default:
                break;
            }
            if (bunka_girl2 == 0) {
                now_serifu = 50;
            }
            bunkaken_param_set2();
        }
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        {
            s32 t8 = girl0_flag;
            s32 t1 = 4;
            s32 t0 = t8 << 29;
            if (t0 < 0) {
                goto_sub_sub_step(10);
            } else {
                u8 t3 = girl0_flag_lo;
                now_serifu = (s16)t1;
                girl0_flag_lo = t3 | (u8)t1;
                sub_sub_step_up();
            }
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        sub_sub_step_up();
        v0 = get_g_zyotai_low(0);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 44;
            break;
        case 2:
            now_serifu = 46;
            break;
        case 3:
            now_serifu = 48;
            break;
        case 4:
            goto_sub_sub_step(12);
        default:
            break;
        }
        t9 = bunka_girl2;
        if (t9 == 0) {
            goto_sub_sub_step(12);
        }
        now_serifu = (s16)(now_serifu + now_year * 8 - 760);
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        v0 = get_g_zyotai_low(0);
        t9 = bunka_girl2;
        if (v0 == 4 || t9 == 0) {
            fade_out_step();
        } else {
            fade_out();
        }
        break;
    case 13:
        v0 = get_g_zyotai_low(0);
        t9 = bunka_girl2;
        if (v0 == 4 || t9 == 0) {
            bunka_sd_kind = 1;
        } else {
            bunka_sd_kind = 2;
        }
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 14:
        bustup_load_step(21713);
        break;
    case 15:
        bunkaken_basyo_init3();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 0;
        zyotai_sprite_set_chk();
        cd_read_start2(23, 0x801E8000, 18923);
        break;
    case 16:
        v0 = get_g_zyotai_low(0);
        t9 = bunka_girl2;
        if (v0 == 4 || t9 == 0) {
            fade_in_step();
        } else {
            fade_in();
        }
        break;
    case 17:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(0);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 45;
                break;
            case 2:
                now_serifu = 47;
                break;
            case 3:
                now_serifu = 49;
                break;
            case 4:
                now_serifu = 51;
            default:
                break;
            }
            if (bunka_girl2 == 0) {
                now_serifu = 51;
            }
            now_serifu = (s16)(now_serifu + now_year * 8 - 760);
            bunkaken_param_set2();
        }
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        v0 = get_g_zyotai_low(0);
        t9 = bunka_girl2;
        if (v0 == 4 || t9 == 0) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 20:
        fade_out_step();
        break;
    case 21:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 22:
        serifu_run();
        break;
    case 23:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
