/* @SymbolStart: 0x80141098 */
/* Address: 0x801410A0 */
/* Size: 1476 bytes (369 insns) */
/* @SymbolSize: 1484 */
/* @RodataBase: 0x801545A0 */
/* @PostProc: swap_instructions --offset1 0x210 --offset2 0x234 */
/* @PostProc: patch_insn_reg_field 0x19c:rt:t3:v0 0x1a8:rt:t3:v0 0x1ac:rt:t4:v0 0x1b8:rt:t4:v0 0x1bc:rt:t5:v0 0x1c8:rt:t5:v0 0x1d4:rt:t6:t3 0x1d8:rs:t6:t3 0x1d8:rt:t6:t3 0x1e0:rs:t6:t3 0x208:rt:v0:t4 0x20c:rs:v0:t4 0x20c:rt:v0:t4 0x210:rt:t9:v0 0x214:rd:t7:t6 0x214:rt:v0:t4 0x218:rs:t7:t6 0x21c:rt:v0:t7 0x230:rs:v0:t7 0x230:rt:v0:t8 0x238:rs:v0:t8 0x238:rt:t8:t9 0x23c:rt:t8:t9 0x248:rt:t9:v0 0x290:rt:t1:v0 0x29c:rt:t1:v0 0x2a0:rt:t2:v0 0x2ac:rt:t2:v0 0x2b0:rt:t3:v0 0x2bc:rt:t3:v0 0x2c0:rt:t4:v0 0x2c8:rt:t4:v0 0x2cc:rt:t5:t1 0x2d0:rs:t5:t1 0x2d0:rt:t5:t1 0x2e0:rs:t5:t1 0x2e4:rt:t6:t2 0x2ec:rs:t6:t2 0x2ec:rt:t6:t2 0x2f4:rd:t7:t3 0x2f4:rt:t6:t2 0x2f8:rd:t8:v0 0x2f8:rs:t7:v0 0x2f8:rt:v0:t3 0x2fc:rs:t8:v0 0x2fc:rt:t9:t5 0x304:rt:t9:t5 0x330:rt:t0:t6 0x334:rs:t0:t6 0x334:rt:t0:t6 0x33c:rs:t0:t6 0x374:rt:t4:t0 0x378:rt:t1:t7 0x37c:rs:t1:t7 0x37c:rt:t1:t7 0x384:rs:t1:t7 0x388:rt:t3:t9 0x38c:rt:t2:t8 0x398:rt:t2:t8 0x3a0:rt:t3:t9 0x3a4:rs:t4:t0 0x3a4:rt:t4:t0 0x3a8:rt:t5:t1 0x3ac:rs:t5:t1 0x3ac:rt:t5:t1 0x3b4:rt:t4:t0 0x3bc:rs:t5:t1 0x3bc:rt:t6:t2 0x3c0:rt:t6:t2 0x3ec:rt:t7:t3 0x3f8:rt:t7:t3 0x3fc:rt:t8:t4 0x408:rt:t8:t4 0x438:rt:t9:t5 0x43c:rs:t9:t5 0x43c:rt:t9:t5 0x444:rs:t9:t5 0x474:rt:t0:t6 0x478:rs:t0:t6 0x490:rd:t1:t7 0x498:rt:t1:t7 0x49c:rt:t1:t7 0x4a4:rs:t1:t7 0x4ac:rt:t2:v0 0x4b8:rt:t2:v0 0x4bc:rt:t3:v0 0x4c8:rt:t3:v0 0x4cc:rt:t4:v0 0x4d8:rt:t4:v0 0x4dc:rt:t5:v0 0x4e4:rt:t5:v0 0x4e8:rt:t6:t8 0x4ec:rs:t6:t8 0x4ec:rt:t6:t8 0x4fc:rs:t6:t8 0x500:rt:t7:t9 0x508:rs:t7:t9 0x508:rt:t7:t9 0x510:rd:t8:t0 0x510:rt:t7:t9 0x514:rd:t9:v0 0x514:rs:t8:v0 0x514:rt:v0:t0 0x518:rs:t9:v0 0x518:rt:t0:t2 0x520:rt:t0:t2 0x54c:rt:t1:t3 0x550:rs:t1:t3 0x550:rt:t1:t3 0x558:rs:t1:t3 */
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
extern void bunkaken_girl2_cl_bank_init(void);
extern void bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

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
extern union { s32 w; u8 b; } girl2_flag;

void bunkaken_girl2_cl_bk2_show_main(void)
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
            bustup_cd_read(21083);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 2;
            zyotai_sprite_set_chk();
            cd_read_start2(9, 0x801E8000, 19000);
        }
        break;
    case 5:
        v0 = get_g_zyotai_low(2);
        t9 = bunka_girl2;
        if (v0 == 4 || t9 == 2) {
            sub_sub_step_up();
        } else {
            fade_up_main();
        }
        break;
    case 6:
        if (get_CD_State2() & 1) {
            bunkaken_girl2_cl_bank_init();
            v0 = get_g_zyotai_low(2);
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
                goto_sub_sub_step(10);
            default:
                break;
            }
            if (bunka_girl2 == 2) {
                goto_sub_sub_step(10);
            }
        }
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        {
            s32 t4 = girl2_flag.w;
            s32 v0b = 4;
            s32 t6 = t4 << 29;
            if (t6 < 0) {
                goto_sub_sub_step(10);
            } else {
                u8 t8 = girl2_flag.b;
                girl2_flag.b = t8 | (u8)v0b;
                now_serifu = (s16)v0b;
                sub_sub_step_up();
            }
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(2);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 5;
            break;
        case 2:
            now_serifu = 6;
            break;
        case 3:
            now_serifu = 7;
            break;
        case 4:
            now_serifu = 8;
            break;
        }
        v0 = now_serifu;
        if (bunka_girl2 == 2) { v0 = 8; }
        now_serifu = (s16)(v0 + now_year * 8 - 760);
        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        v0 = get_g_zyotai_low(2);
        t9 = bunka_girl2;
        if (v0 == 4 || t9 == 2) {
            fade_out_step();
        } else {
            fade_out();
        }
        break;
    case 13:
        v0 = get_g_zyotai_low(2);
        t9 = bunka_girl2;
        if (v0 == 4 || t9 == 2) {
            bunka_sd_kind = 1;
        } else {
            bunka_sd_kind = 3;
        }
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 14:
        bustup_load_step(21083);
        break;
    case 15:
        bunkaken_basyo_init3();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 2;
        zyotai_sprite_set_chk();
        cd_read_start2(9, 0x801E8000, 19000);
        break;
    case 16:
        v0 = get_g_zyotai_low(2);
        t9 = bunka_girl2;
        if (v0 == 4 || t9 == 2) {
            fade_in_step();
        } else {
            fade_in();
        }
        break;
    case 17:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(2);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 9;
                break;
            case 2:
                now_serifu = 10;
                break;
            case 3:
                now_serifu = 11;
                break;
            case 4:
                now_serifu = 12;
                break;
            }
            v0 = now_serifu;
            if (bunka_girl2 == 2) { v0 = 12; }
            now_serifu = (s16)(v0 + now_year * 8 - 760);
            bunkaken_girl2_cl_bank_init();
        }
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        v0 = get_g_zyotai_low(2);
        t9 = bunka_girl2;
        if (v0 == 4 || t9 == 2) {
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
