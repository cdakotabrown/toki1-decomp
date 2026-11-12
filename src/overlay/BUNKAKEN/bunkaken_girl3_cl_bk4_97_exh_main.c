/* @SymbolStart: 0x801434A4 */
/* @SymbolSize: 1124 */
/* @RodataBase: 0x801548A8 */
/* @PostProc: patch_insn_reg_field 0x240:rt:t0:t1 0x244:rd:t9:t0 0x248:rs:t9:t0 0x264:rt:t0:t1 0x29c:rt:t2:t3 0x2a8:rt:t3:t4 0x2b4:rt:t1:t2 0x2c0:rt:t1:t2 0x2cc:rt:t2:t3 0x2d4:rt:t3:t4 0x308:rt:t4:t5 0x30c:rs:t4:t5 0x30c:rt:t4:t5 0x314:rs:t4:t5 0x35c:rt:t5:t6 0x360:rs:t5:t6 0x360:rt:t5:t6 0x368:rs:t5:t6 0x3b0:rt:t6:t7 0x3b4:rs:t6:t7 0x3b4:rt:t6:t7 0x3bc:rs:t6:t7 0x3f4:rt:t7:t8 0x3f8:rs:t7:t8 0x3f8:rt:t7:t8 0x400:rs:t7:t8 */
/* Address: 0x801434AC (window starts at the absorbed cross-prologue head */
/* Size: 1116 bytes (279 insns) + 8-byte head = 1124 */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 goto_sub_sub_step();
extern s32 get_CD_State2();
extern s32 cd_read_start2();
extern s32 game_reset();
extern s32 indexed_byte_lsb_set();
extern s32 value_cache_find_or_load();
extern s32 dec_bg_init();
extern s32 bustup_cd_read();
extern s32 bustup_ptr_init();
extern s32 fade_up_main();
extern s32 fade_down_main();
extern s32 fade_in_step();
extern s32 fade_in();
extern s32 fade_out_step();
extern s32 fade_out();
extern s32 zyotai_sprite_set_chk();
extern s32 serifu_run();
extern s32 get_g_zyotai_low();
extern s32 bunkaken_girl3_cl_bank_init();
extern s32 bunkaken_sub_reset();
extern void bunkaken_basyo_init3();

extern s16 now_serifu;
extern s32 girl3_flag;
extern u8  now_girl;
extern u8  now_sub_sub_step;
extern s32 bunka_girl2;

void bunkaken_girl3_cl_bk4_97_exh_main(void) {
    s32 sel = now_sub_sub_step;

    switch ((u32)sel) {
        case 0:
            value_cache_find_or_load(0x410F, 1);
            sub_sub_step_up();
            break;

        case 1:
            fade_out_step();
            break;

        case 2:
            if (get_CD_State2() & 1) {
                bunkaken_basyo_init3();
                indexed_byte_lsb_set(1, dec_bg_init(1));
                bustup_cd_read(0x58DC);
                sub_sub_step_up();
            }
            break;

        case 3:
            fade_in_step();
            break;

        case 4:
            if (get_CD_State2() & 1) {
                bustup_ptr_init();
                now_girl = 3;
                zyotai_sprite_set_chk();
                cd_read_start2(9, 0x801E8000, 0x4B0A);
            }
            break;

        case 5: {
            s32 ret;
            ret = get_g_zyotai_low(3);
            if (ret == 4 || bunka_girl2 == 3) {
                sub_sub_step_up();
            } else {
                fade_up_main();
            }
            break;
        }

        case 6:
            if (get_CD_State2() & 1) {
                s32 ret;
                bunkaken_girl3_cl_bank_init();
                switch (get_g_zyotai_low(3)) {
                    case 0:
                    case 1:
                        now_serifu = 48;
                        break;
                    case 2:
                        now_serifu = 52;
                        break;
                    case 3:
                        now_serifu = 56;
                        break;
                }
                ret = get_g_zyotai_low(3);
                if (ret == 4 || bunka_girl2 == 3) {
                    now_serifu = 60;
                }
            }
            break;

        case 7:
            serifu_run();
            break;

        case 8: {
            s32 ret;
            ret = get_g_zyotai_low(3);
            if (ret == 4 || bunka_girl2 == 3) {
                goto_sub_sub_step(12);
            } else {
                s32 val;
                val = girl3_flag;
                if ((val << 29) < 0) {
                    goto_sub_sub_step(10);
                } else {
                    now_serifu = 1;
                    sub_sub_step_up();
                }
            }
            break;
        }

        case 9:
            serifu_run();
            break;

        case 10:
            switch (get_g_zyotai_low(3)) {
                case 0:
                case 1:
                    now_serifu = 49;
                    break;
                case 2:
                    now_serifu = 53;
                    break;
                case 3:
                    now_serifu = 57;
                    break;
            }
            sub_sub_step_up();
            break;

        case 11:
            serifu_run();
            break;

        case 12: {
            s32 ret;
            ret = get_g_zyotai_low(3);
            if (ret == 4 || bunka_girl2 == 3) {
                fade_out_step();
            } else {
                fade_out();
            }
            break;
        }

        case 13:
            serifu_run();
            break;

        case 14: {
            s32 ret;
            ret = get_g_zyotai_low(3);
            if (ret == 4 || bunka_girl2 == 3) {
                fade_in_step();
            } else {
                fade_in();
            }
            break;
        }

        case 15:
            serifu_run();
            break;

        case 16: {
            s32 ret;
            ret = get_g_zyotai_low(3);
            if (ret == 4 || bunka_girl2 == 3) {
                fade_out_step();
            } else {
                fade_down_main();
            }
            break;
        }

        case 17: {
            s32 ret;
            ret = get_g_zyotai_low(3);
            if (ret == 4 || bunka_girl2 == 3) {
                sub_sub_step_up();
            } else {
                fade_out_step();
            }
            break;
        }

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
