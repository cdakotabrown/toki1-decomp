/* @SymbolStart: 0x80151F10 */
/* @RodataBase: 0x8015F35C */
/* Address: 0x80151F18 */
/* Size: 1160 bytes (290 insns) */
/* @SymbolSize: 1168 */
/* @PostProc: swap_instructions --offset1 0x3ec --offset2 0x3f0 */
/* @PostProc: patch_insn_reg_field 0x14c:rt:t1:v1 0x158:rt:t1:v1 0x15c:rt:t2:v1 0x168:rt:t2:v1 0x16c:rt:t3:v1 0x178:rt:t3:v1 0x17c:rt:t4:v1 0x184:rt:t4:v1 0x188:rt:v1:v0 0x18c:rs:v1:v0 0x18c:rt:v1:v0 0x190:rt:v0:v1 0x194:rs:v0:v1 0x194:rt:v0:v1 0x19c:rs:v1:v0 0x1a4:rs:v0:v1 0x1a4:rt:v0:v1 0x1a8:rd:t5:t1 0x1a8:rt:v0:v1 0x1ac:rd:v0:v1 0x1ac:rt:t5:t1 0x1b4:rt:v0:v1 0x1bc:rs:v1:v0 0x1c0:rs:v0:v1 0x1c0:rt:t7:t4 0x1c8:rt:t7:t4 0x1fc:rt:t8:t5 0x204:rt:t9:t6 0x208:rs:t9:t6 0x208:rt:t9:t6 0x20c:rt:t8:t5 0x214:rt:t9:t6 0x21c:rt:t0:t7 0x220:rt:t0:t7 0x24c:rt:t1:t8 0x250:rs:t1:t8 0x260:rt:t2:t9 0x26c:rt:t2:t9 0x2b0:rt:t3:t0 0x2b4:rs:t3:t0 0x2b8:rt:t4:v0 0x2bc:rs:t4:v0 0x2bc:rt:t4:v0 0x2c4:rs:t4:v0 0x2dc:rd:t5:t1 0x2e4:rt:t5:t1 0x2e8:rt:t5:t1 0x2f0:rs:t5:t1 0x2f8:rt:t6:v1 0x304:rt:t6:v1 0x308:rt:t7:v1 0x314:rt:t7:v1 0x318:rt:t8:v1 0x324:rt:t8:v1 0x328:rt:t9:v1 0x334:rt:t9:v1 0x338:rt:t0:v1 0x340:rt:t0:v1 0x344:rt:t1:t2 0x348:rs:t1:t2 0x348:rt:t1:t2 0x350:rs:t1:t2 0x36c:rt:t2:v1 0x370:rs:t2:v1 0x370:rt:t2:v1 0x378:rs:t2:v1 0x378:rt:t3:t4 0x380:rt:t3:t4 0x394:rd:t4:t5 0x39c:rt:t4:t5 0x3a0:rt:t4:t5 0x3a8:rs:t4:t5 0x3b0:rt:t5:v1 0x3bc:rt:t5:v1 0x3c0:rt:t6:v1 0x3cc:rt:t6:v1 0x3d0:rt:t7:v1 0x3dc:rt:t7:v1 0x3e0:rt:t8:v1 0x3e8:rt:t8:v1 0x3ec:rt:t9:v0 0x3f0:rt:v0:v1 0x3f4:rs:v0:v1 0x3f4:rt:v0:v1 0x3f8:rs:t9:v0 0x3f8:rt:t9:v0 0x400:rs:t9:v0 0x404:rs:v0:v1 0x404:rt:t0:t7 0x40c:rt:t0:t7 0x450:rt:t1:v1 0x45c:rt:t1:v1 */
#include "types.h"

extern u8 now_sub_sub_step;
extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32  dec_bg_init(s32);
extern void bustup_cd_read(s32);
extern void bustup_ptr_init(void);
extern void fade_down_main(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32  get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl0_home_srf_bank_set(void);

extern volatile s16 now_serifu;
extern u8  now_year;
extern u16 bukatu_pt2;
extern u8  now_girl;
extern u8  now_sub_step;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkasai_girl0_home_club2_scene(void) {
    s32 v0;
    s32 n;
    s32 b;
    switch (now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16636, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bustup_cd_read(21713);
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 3:
        if (get_CD_State2() & 1) {
            bunkasai_basyo_init();
            bustup_ptr_init();
            now_girl = 0;
            zyotai_sprite_set_chk();
            cd_read_start2(24, 0x801E8000, 18896);
        }
        break;
    case 4:
        fade_in();
        break;
    case 5:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(0);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 52;
                break;
            case 2:
                now_serifu = 53;
                break;
            case 3:
                now_serifu = 54;
                break;
            case 4:
                now_serifu = 55;
                break;
            }
            v0 = now_year;
            n = now_serifu;
            if (v0 == 96) {
                n = (s16) (n + 12);
            }
            now_serifu = n;
            if (v0 == 97) {
                now_serifu = n + 20;
            }
            bunkasai_girl0_home_srf_bank_set();
        }
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        fade_out();
        break;
    case 8:
        bunka_sd_kind = 2;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 9:
        bustup_load_step(21713);
        break;
    case 10:
        if (get_CD_State2() & 1) {
            bunkasai_basyo_init();
            mes_base_y = 192;
            bustup_ptr_init();
            now_girl = 0;
            zyotai_sprite_set_chk();
            cd_read_start2(24, 0x801E8000, 18896);
        }
        break;
    case 11:
        fade_in();
        break;
    case 12:
        if (get_CD_State2() & 1) {
            if (now_year == 95) {
                v0 = get_g_zyotai_low(0);
                switch (v0) {
                case 0:
                    now_serifu = 57;
                    break;
                case 1:
                    now_serifu = 56;
                    break;
                case 2:
                    now_serifu = 58;
                    break;
                case 3:
                    now_serifu = 60;
                    break;
                case 4:
                    now_serifu = 62;
                    break;
                }
                if ((u32) bukatu_pt2 < 100) {
                    v0 = get_g_zyotai_low(0);
                    if ((u32) v0 >= 2) {
                        now_serifu = now_serifu + 1;
                    }
                }
            } else {
                v0 = get_g_zyotai_low(0);
                switch (v0) {
                case 0:
                case 1:
                    now_serifu = 68;
                    break;
                case 2:
                    now_serifu = 69;
                    break;
                case 3:
                    now_serifu = 70;
                    break;
                case 4:
                    now_serifu = 71;
                    break;
                }
                b = now_year;
                n = now_serifu;
                if (b == 97) {
                    now_serifu = n + 8;
                }
            }
            bunkasai_girl0_home_srf_bank_set();
        }
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        fade_down_main();
        break;
    case 15:
        fade_out_step();
        break;
    case 16:
        now_serifu = 1;
        sub_sub_step_up();
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
