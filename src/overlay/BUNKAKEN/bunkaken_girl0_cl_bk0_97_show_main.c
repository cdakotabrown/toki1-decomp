/* @SymbolStart: 0x8014AF88 */
/* @SymbolSize: 1072 */
/* @RodataBase: 0x801552CC */
/* @PostProc: patch_insn_reg_field 0xec:rt:t2:t3 0xf0:rd:t0:t1 0xf4:rs:t0:t1 0xfc:rt:t1:t2 0x108:rt:t1:t2 0x10c:rt:t2:t3 0x140:rt:t3:t4 0x148:rt:t4:t5 0x14c:rs:t4:t5 0x14c:rt:t4:t5 0x150:rt:t3:t4 0x158:rt:t4:t5 0x160:rt:t5:t6 0x164:rt:t5:t6 0x1ac:rt:t6:t7 0x1b0:rs:t6:t7 0x1c8:rt:v1:t8 0x1cc:rt:t9:t4 0x1d0:rd:t7:t0 0x1d0:rt:v1:t8 0x1d4:rs:t7:t0 0x1ec:rt:v1:t2 0x1f4:rs:v1:t2 0x1f4:rt:t8:t3 0x1f8:rt:t8:t3 0x1fc:rt:t9:t4 0x224:rt:t0:t5 0x230:rt:t0:t5 0x254:rt:t1:t6 0x258:rs:t1:t6 0x278:rt:t2:t7 0x27c:rs:t2:t7 0x2cc:rt:t3:t8 0x2d0:rs:t3:t8 0x2d0:rt:t3:t8 0x2d8:rs:t3:t8 0x310:rd:t4:t9 0x318:rt:t4:t9 0x31c:rt:t4:t9 0x324:rs:t4:t9 0x32c:rt:t5:t0 0x338:rt:t5:t0 0x33c:rt:t6:t1 0x348:rt:t6:t1 0x34c:rt:t7:t2 0x358:rt:t7:t2 0x35c:rt:t8:t3 0x364:rt:t8:t3 0x368:rt:t9:t4 0x36c:rs:t9:t4 0x36c:rt:t9:t4 0x370:rt:t0:t5 0x374:rs:t9:t4 0x37c:rt:t0:t5 0x3b0:rt:t1:t6 0x3b4:rs:t1:t6 0x3b4:rt:t1:t6 0x3bc:rs:t1:t6 */
/* Address: 0x8014AF90 (window starts at the absorbed cross-prologue head */
/* Size: 1064 bytes (266 insns) + 8-byte head = 1072 */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 goto_step();
extern s32 goto_sub_sub_step();
extern s32 get_CD_State2();
extern s32 cd_read_start2();
extern s32 game_reset();
extern s32 indexed_byte_lsb_set();
extern s32 value_cache_find_or_load();
extern s32 dec_bg_init();
extern s32 bustup_ptr_init();
extern s32 fade_down_main();
extern s32 fade_in_step();
extern s32 fade_in();
extern s32 fade_out_step();
extern s32 zyotai_sprite_set_chk();
extern s32 serifu_run();
extern s32 get_g_zyotai_low();
extern s32 bustup_load_step();
extern s32 bunkaken_sub_reset();
extern void bunkaken_basyo_init();
extern void bunkaken_basyo_init2();
extern void bunkaken_basyo_init3();
extern void bunkaken_param_set2();

extern s16 now_serifu;
extern u8  screen_w[];
extern s32 girl0_flag;
extern u8  now_girl;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern s32 bunka_girl2;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;

void bunkaken_girl0_cl_bk0_97_show_main(void) {
    s32 sel = now_sub_sub_step;

    switch ((u32)sel) {
        case 0:
            value_cache_find_or_load(18044, 1);
            sub_sub_step_up();
            break;

        case 1:
            fade_out_step();
            break;

        case 2:
            if (get_CD_State2() & 1) {
                bunkaken_basyo_init3();
                indexed_byte_lsb_set(1, dec_bg_init(1));
                cd_read_start2(23, 0x801E8000, 18923);
                sub_sub_step_up();
            }
            break;

        case 3:
            fade_in_step();
            break;

        case 4:
            if (get_CD_State2() & 1) {
                s32 val;
                val = girl0_flag;
                if ((val << 29) < 0) {
                    now_serifu = 25;
                } else {
                    now_serifu = 26;
                }
                bunkaken_param_set2();
            }
            break;

        case 5:
            serifu_run();
            break;

        case 6:
            fade_out_step();
            break;

        case 7:
            bunka_sd_kind = 2;
            bunka_ret_sub_step = now_sub_step;
            bunka_ret_sub_sub_step = now_sub_sub_step + 1;
            goto_step(128);
            break;

        case 8:
            bunkaken_basyo_init2();
            cd_read_start2(23, 0x801E8000, 18923);
            sub_sub_step_up();
            break;

        case 9:
            if (get_CD_State2() & 1) {
                u8 *base;
                s32 val;
                u8 b;
                bunkaken_param_set2();
                base = (u8*)((u32)screen_w | 0);
                val = *(s32*)(base + 456);
                if ((val << 29) < 0) {
                    goto_sub_sub_step(11);
                } else {
                    b = base[456];
                    base[456] = b | 4;
                    now_serifu = 4;
                }
                value_cache_find_or_load(16628, 1);
            }
            break;

        case 10:
            serifu_run();
            break;

        case 11:
            now_serifu = 6;
            sub_sub_step_up();
            break;

        case 12:
            serifu_run();
            break;

        case 13:
            if (get_CD_State2() & 1) {
                bustup_load_step(21578);
            }
            break;

        case 14:
            if (get_CD_State2() & 1) {
                indexed_byte_lsb_set(1, dec_bg_init(1));
                bunkaken_basyo_init();
                bustup_ptr_init();
                now_girl = 0;
                zyotai_sprite_set_chk();
            }
            break;

        case 15: {
            s32 ret;
            ret = get_g_zyotai_low(0);
            if (ret == 4 || bunka_girl2 == 0) {
                fade_in_step();
            } else {
                fade_in();
            }
            break;
        }

        case 16:
            switch (get_g_zyotai_low(0)) {
                case 0:
                case 1:
                    now_serifu = 27;
                    break;
                case 2:
                    now_serifu = 28;
                    break;
                case 3:
                    now_serifu = 29;
                    break;
                case 4:
                    now_serifu = 30;
                    break;
            }
            if (bunka_girl2 == 0) {
                now_serifu = 30;
            }
            sub_sub_step_up();
            break;

        case 17:
            serifu_run();
            break;

        case 18: {
            s32 ret;
            ret = get_g_zyotai_low(0);
            if (ret == 4 || bunka_girl2 == 0) {
                sub_sub_step_up();
            } else {
                fade_down_main();
            }
            break;
        }

        case 19:
            fade_out_step();
            break;

        case 20:
            now_serifu = 0;
            sub_sub_step_up();
            break;

        case 21:
            serifu_run();
            break;

        case 22:
            bunkaken_sub_reset();
            break;

        default:
            game_reset();
            break;
    }
}
