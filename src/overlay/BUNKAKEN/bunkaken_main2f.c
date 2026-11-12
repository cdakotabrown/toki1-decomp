/* @SymbolStart: 0x8013CA6C */
/* @SymbolSize: 1108 */
/* @RodataBase: 0x80153fc4 */
/* @PostProc: swap_instructions --offset1 0x350 --offset2 0x374 */
/* @PostProc: swap_instructions --offset1 0x370 --offset2 0x374 */
/* @PostProc: swap_instructions --offset1 0x400 --offset2 0x404 */
/* @PostProc: swap_instructions --offset1 0x408 --offset2 0x40c */
/* @PostProc: patch_insn_reg_field 0x188:rt:t3:v0 0x18c:rs:t3:v0 0x18c:rt:t3:v0 0x190:rs:t3:v0 0x190:rt:v1:t3 0x194:rt:t5:t6 0x198:rd:t4:t5 0x198:rt:v1:t3 0x19c:rs:t4:t5 0x1b4:rt:t6:t7 0x1b8:rs:t6:t7 0x1b8:rt:t6:t8 0x1bc:rt:t5:t6 0x1c4:rs:t6:t8 0x1c4:rt:t7:t9 0x1c8:rt:t7:t9 0x200:rt:t8:t0 0x20c:rt:t8:t0 0x230:rt:t9:t1 0x234:rs:t9:t1 0x270:rt:t0:t2 0x274:rs:t0:t2 0x28c:rt:t1:t3 0x298:rt:t1:t3 0x2b4:rt:t2:t4 0x2b8:rs:t2:t4 0x2b8:rt:t2:t4 0x2c0:rs:t2:t4 0x304:rt:t4:t6 0x310:rt:t5:t7 0x31c:rt:t3:t5 0x328:rt:t3:t5 0x334:rt:t4:t6 0x33c:rt:t5:t7 0x354:rt:t6:t8 0x358:rs:t6:t8 0x358:rt:t6:t8 0x360:rs:t6:t8 0x364:rt:t7:t9 0x36c:rt:t7:t9 0x370:rt:t8:t1 0x374:rs:t8:t1 0x374:rt:t8:t1 0x37c:rd:t9:t2 0x37c:rt:t8:t1 0x380:rd:t1:t3 0x380:rs:t9:t0 0x380:rt:t0:t2 0x384:rs:t1:t3 0x384:rt:t2:t4 0x390:rt:t2:t4 0x3bc:rt:t3:t5 0x3c0:rs:t3:t5 0x3c0:rt:t3:t5 0x3c8:rs:t3:t5 0x410:rt:v1:t7 0x418:rs:v1:t7 0x418:rt:t4:t8 0x420:rt:t4:t8 */
/* Address: 0x8013CA74 (window starts at the absorbed cross-prologue head */
/* Size: 1100 bytes (275 insns) + 8-byte head = 1108 */
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
extern s32 bustup_cd_read();
extern s32 bustup_ptr_init();
extern s32 fade_down_main();
extern s32 fade_in_step();
extern s32 fade_in();
extern s32 fade_out_step();
extern s32 zyotai_sprite_set_chk();
extern s32 serifu_run();
extern s32 get_g_zyotai_low();
extern s32 bunkaken_girl1_cl_bank_init();
extern s32 bunkaken_sub_reset();
extern void bunkaken_basyo_init3();
extern void bunkaken_basyo_init();

extern s16 now_serifu;
extern u8  screen_w[];
extern u8  girl1_flag;
extern u8  now_year;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern u8  now_girl;
extern s32 bunka_girl2;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;

void bunkaken_main2f(void) {
    s32 sel = now_sub_sub_step;

    switch ((u32)sel) {
        case 0:
            value_cache_find_or_load(0x467C, 1);
            sub_sub_step_up();
            break;

        case 1:
            fade_out_step();
            break;

        case 2:
            if (get_CD_State2() & 1) {
                bunkaken_basyo_init3();
                indexed_byte_lsb_set(1, dec_bg_init(1));
                cd_read_start2(7, 0x801E8000, 0x4AB6);
                sub_sub_step_up();
            }
            break;

        case 3:
            fade_in_step();
            break;

        case 4:
            if (get_CD_State2() & 1) {
                now_serifu = 31;
                bunkaken_girl1_cl_bank_init();
            }
            break;

        case 5:
            serifu_run();
            break;

        case 6:
            fade_out_step();
            break;

        case 7: {
            s32 t0;
            s32 t1;
            t0 = now_sub_step;
            bunka_ret_sub_step = t0;
            t1 = now_sub_sub_step + 1;
            bunka_ret_sub_sub_step = t1;
            goto_step(128);
            break;
        }

        case 8:
            cd_read_start2(7, 0x801E8000, 0x4AB6);
            sub_sub_step_up();
            break;

        case 9:
            if (get_CD_State2() & 1) {
                u8 *base;
                s32 val;
                u8 b;
                bunkaken_girl1_cl_bank_init();
                base = (u8*)((u32)screen_w | 0);
                val = *(s32*)(base + 0x200);
                if ((val << 29) < 0) {
                    goto_sub_sub_step(11);
                } else {
                    b = girl1_flag;
                    now_serifu = 5;
                    girl1_flag = b | 4;
                }
                dec_bg_reset();
                set_dec_bri(0);
                value_cache_find_or_load(0x40F4, 1);
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
                indexed_byte_lsb_set(1, dec_bg_init(1));
                bustup_cd_read(0x5D6E);
                sub_sub_step_up();
            }
            break;

        case 14:
            if (get_CD_State2() & 1) {
                bunkaken_basyo_init();
                bustup_ptr_init();
                now_girl = 1;
                zyotai_sprite_set_chk();
            }
            break;

        case 15: {
            s32 ret;
            ret = get_g_zyotai_low(1);
            if (ret == 4 || bunka_girl2 == 1) {
                fade_in_step();
            } else {
                fade_in();
            }
            break;
        }

        case 16: {
            s32 ret;
            s32 ret2;
            s32 current;
            s32 t1;
            ret = get_g_zyotai_low(1);
            switch (ret) {
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
            }
            ret2 = get_g_zyotai_low(1);
            if (ret2 == 4 || bunka_girl2 == 1) {
                now_serifu = 35;
            }
            t1 = now_year;
            current = now_serifu;
            now_serifu = current + t1 * 4 - 380;
            sub_sub_step_up();
            break;
        }

        case 17:
            serifu_run();
            break;

        case 18: {
            s32 ret;
            ret = get_g_zyotai_low(1);
            if (ret == 4 || bunka_girl2 == 1) {
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
            s32 val;
            base = (u8*)((u32)screen_w | 0);
            now_serifu = 0;
            val = base[0x200];
            base[0x200] = val | 4;
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
