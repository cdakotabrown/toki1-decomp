/* @SymbolStart: 0x8014B3B8 */
/* @SymbolSize: 1088 */
/* @RodataBase: 0x8015533C */
/* @PostProc: swap_instructions --offset1 0x19c --offset2 0x1c0 */
/* @PostProc: swap_instructions --offset1 0x1a8 --offset2 0x1c0 */
/* @PostProc: patch_insn_reg_field 0xe4:rt:t9:v1 0xf0:rt:t9:v1 0x11c:rt:t0:t9 0x120:rs:t0:t9 0x120:rt:t0:t9 0x128:rt:t0:t9 0x130:rt:t1:t0 0x134:rt:t1:t0 0x17c:rt:t2:t1 0x180:rs:t2:t1 0x190:rt:t3:v0 0x194:rs:t3:v0 0x194:rt:t3:v0 0x198:rs:t3:v0 0x198:rt:v0:t2 0x19c:rt:t7:v1 0x1a0:rt:v0:t2 0x1bc:rt:t5:t6 0x1c4:rs:t5:t6 0x1c4:rt:t6:t7 0x1c8:rt:t6:t7 0x1d0:rt:t7:v1 0x208:rt:t8:v1 0x214:rt:t8:v1 0x238:rt:t9:t8 0x23c:rs:t9:t8 0x25c:rt:t0:t9 0x260:rs:t0:t9 0x2b0:rt:t1:t0 0x2b4:rs:t1:t0 0x2b4:rt:t1:t0 0x2bc:rs:t1:t0 0x2f4:rd:t2:t1 0x2fc:rt:t2:t1 0x300:rt:t2:t1 0x308:rs:t2:t1 0x310:rt:t3:v1 0x31c:rt:t3:v1 0x320:rt:t4:v1 0x32c:rt:t4:v1 0x330:rt:t5:v1 0x33c:rt:t5:v1 0x340:rt:t6:v1 0x348:rt:t6:v1 0x34c:rt:t7:t2 0x350:rs:t7:t2 0x350:rt:t7:t2 0x354:rt:v0:v1 0x358:rs:v0:v1 0x358:rt:v0:v1 0x35c:rs:t7:t2 0x360:rt:t8:t3 0x364:rt:v0:v1 0x368:rs:t8:t3 0x368:rt:t8:t3 0x370:rd:t9:t4 0x370:rt:t8:t3 0x374:rd:t0:v1 0x374:rs:t9:v1 0x374:rt:v0:t4 0x378:rs:t0:v1 0x378:rt:t1:t6 0x380:rt:t1:t6 0x3ac:rt:t2:t7 0x3b0:rs:t2:t7 0x3b0:rt:t2:t7 0x3b8:rs:t2:t7 0x3f8:rt:v1:t9 0x400:rs:v1:t9 0x400:rt:t3:t0 0x404:rt:t3:t0 */
/* Address: 0x8014B3C0 (window starts at the absorbed cross-prologue head */
/* Size: 1080 bytes (270 insns) + 8-byte head = 1088 */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 goto_step();
extern s32 goto_sub_sub_step();
extern s32 get_CD_State2();
extern s32 cd_read_start2();
extern s32 game_reset();
extern s32 set_dec_bri();
extern s32 dec_bg_reset();
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
extern u8  now_year;
extern u8  girl0_flag;
extern u8  now_girl;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern s32 bunka_girl2;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;

void bunkaken_girl0_cl_bk1_show_main(void) {
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
                now_serifu = 31;
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
                    b = girl0_flag;
                    girl0_flag = b | 4;
                    now_serifu = 5;
                }
                dec_bg_reset();
                set_dec_bri(0);
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
                bustup_load_step(21623);
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

        case 16: {
            s32 cur;
            switch (get_g_zyotai_low(0)) {
                case 0:
                case 1:
                    now_serifu = 32;
                    break;
                case 2:
                    now_serifu = 33;
                    break;
                case 3:
                    now_serifu = 34;
                    break;
                case 4:
                    now_serifu = 35;
                    break;
            }
            cur = now_serifu;
            if (bunka_girl2 == 0) {
                cur = 35;
            }
            now_serifu = cur + now_year * 4 - 380;
            sub_sub_step_up();
            break;
        }

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

        case 20: {
            u8 *base;
            u8 b;
            base = (u8*)((u32)screen_w | 0);
            b = base[456];
            base[456] = b | 4;
            now_serifu = 0;
            sub_sub_step_up();
            break;
        }

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
