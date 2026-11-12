/* @SymbolStart: 0x801422D4 */
/* @SymbolSize: 1440 */
/* @RodataBase: 0x80154738 */
/* Address: 0x801422DC */
/* Size: 1432 bytes (358 insns) */
/* @PostProc: patch_insn_reg_field 0x218:rd:t7:t8 0x214:rt:t8:t9 0x21c:rs:t7:t8 0x234:rt:t9:t0 0x238:rs:t9:t0 0x238:rt:t9:t1 0x23c:rt:t8:t9 0x244:rs:t9:t1 0x244:rt:t0:t2 0x24c:rt:t0:t2 0x284:rt:t2:t4 0x290:rt:t3:t5 0x29c:rt:t1:t3 0x2a8:rt:t1:t3 0x2b4:rt:t2:t4 0x2bc:rt:t3:t5 0x2d0:rt:t4:t6 0x2d4:rs:t4:t6 0x2d4:rt:t4:t6 0x2dc:rs:t4:t6 0x2e0:rt:t5:t7 0x2e8:rt:t5:t7 0x31c:rt:t6:t8 0x320:rs:t6:t8 0x320:rt:t6:t8 0x328:rs:t6:t8 0x360:rt:t0:t2 0x364:rt:t7:t9 0x368:rs:t7:t9 0x368:rt:t7:t9 0x370:rs:t7:t9 0x374:rt:t9:t1 0x378:rt:t8:t0 0x384:rt:t8:t0 0x38c:rt:t9:t1 0x390:rs:t0:t2 0x390:rt:t0:t2 0x394:rt:t1:t3 0x398:rs:t1:t3 0x398:rt:t1:t3 0x3a0:rt:t0:t2 0x3a8:rs:t1:t3 0x3a8:rt:t2:t4 0x3ac:rt:t2:t4 0x3d8:rt:t3:t5 0x3e4:rt:t3:t5 0x3e8:rt:t4:t6 0x3f4:rt:t4:t6 0x424:rt:t5:t7 0x428:rs:t5:t7 0x428:rt:t5:t7 0x430:rs:t5:t7 0x460:rt:t6:t8 0x464:rs:t6:t8 0x488:rt:t8:t0 0x494:rt:t9:t1 0x4a0:rt:t7:t9 0x4ac:rt:t7:t9 0x4b8:rt:t8:t0 0x4c0:rt:t9:t1 0x4d4:rt:t0:t2 0x4d8:rs:t0:t2 0x4d8:rt:t0:t2 0x4e0:rs:t0:t2 0x4e4:rt:t1:t3 0x4ec:rt:t1:t3 0x520:rt:t2:t4 0x524:rs:t2:t4 0x524:rt:t2:t4 0x52c:rs:t2:t4 */
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
extern s16 now_serifu;
extern u8  now_girl;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern s32 girl2_flag;
extern u8  girl2_flag_b;

void bunkaken_girl2_cl_bk3_97_show_main(void)
{
    s32 idx = now_sub_sub_step;
    s32 v0;
    s32 ret;

    switch (idx) {
    case 0:
        value_cache_find_or_load(16645, 1);
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
            bustup_cd_read(21038);
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
        if (v0 == 4 || bunka_girl2 == 2) {
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
            }
            v0 = get_g_zyotai_low(2);
            if (v0 == 4 || bunka_girl2 == 2) {
                goto_sub_sub_step(10);
            }
        }
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        {
            s32 t4 = girl2_flag;
            s32 v0b = 4;
            s32 t6 = t4 << 29;
            if (t6 < 0) {
                goto_sub_sub_step(10);
            } else {
                u8 t8 = girl2_flag_b;
                now_serifu = (s16)v0b;
                girl2_flag_b = t8 | (u8)v0b;
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
            now_serifu = 53;
            break;
        case 2:
            now_serifu = 54;
            break;
        case 3:
            now_serifu = 55;
            break;
        }
        v0 = get_g_zyotai_low(2);
        if (v0 == 4 || bunka_girl2 == 2) {
            now_serifu = 56;
        }
        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        v0 = get_g_zyotai_low(2);
        if (v0 == 4 || bunka_girl2 == 2) {
            fade_out_step();
        } else {
            fade_out();
        }
        break;
    case 13:
        v0 = get_g_zyotai_low(2);
        if (v0 == 4 || bunka_girl2 == 2) {
            bunka_sd_kind = 1;
        } else {
            bunka_sd_kind = 3;
        }
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 14:
        bustup_load_step(21038);
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
        if (v0 == 4 || bunka_girl2 == 2) {
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
                now_serifu = 57;
                break;
            case 2:
                now_serifu = 58;
                break;
            case 3:
                now_serifu = 59;
                break;
            }
            v0 = get_g_zyotai_low(2);
            if (v0 == 4 || bunka_girl2 == 2) {
                now_serifu = 60;
            }
            bunkaken_girl2_cl_bank_init();
        }
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        v0 = get_g_zyotai_low(2);
        if (v0 == 4 || bunka_girl2 == 2) {
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
