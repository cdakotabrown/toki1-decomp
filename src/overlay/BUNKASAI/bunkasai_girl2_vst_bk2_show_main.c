/* @SymbolStart: 0x801333CC */
/* Address: 0x801333D4 */
/* Size: 1468 bytes (367 insns) */
/* @SymbolSize: 1476 */
/* @RodataBase: 0x8015BACC */
/* @PostProc: patch_insn_reg_field 0x210:rt:t2:t3 0x214:rd:t1:t2 0x218:rs:t1:t2 0x220:rs:t2:t3 0x220:rt:t2:t3 0x228:rs:t2:t3 0x240:rd:t3:t4 0x248:rt:t3:t4 0x24c:rt:t3:t4 0x254:rs:t3:t4 0x25c:rt:t4:t5 0x268:rt:t4:t5 0x26c:rt:t5:t6 0x278:rt:t5:t6 0x27c:rt:t6:t7 0x288:rt:t6:t7 0x28c:rt:t7:t8 0x294:rt:t7:t8 0x2c8:rt:t8:t9 0x2cc:rs:t8:t9 0x2cc:rt:t8:t9 0x2d4:rs:t8:t9 0x2ec:rd:t9:t0 0x2f4:rt:t9:t0 0x2f8:rt:t9:t0 0x300:rs:t9:t0 0x308:rt:t0:t1 0x314:rt:t0:t1 0x318:rt:t1:t2 0x324:rt:t1:t2 0x328:rt:t2:t3 0x334:rt:t2:t3 0x338:rt:t3:t4 0x344:rt:t3:t4 0x358:rt:t5:t6 0x35c:rt:t4:t5 0x368:rt:t4:t5 0x370:rt:t5:t6 0x3a8:rt:t6:t7 0x3ac:rs:t6:t7 0x3ac:rt:t6:t7 0x3b8:rt:t6:t7 0x3c0:rt:t7:t8 0x3c4:rt:t7:t8 0x3f0:rt:t8:t9 0x3f4:rs:t8:t9 0x404:rt:t9:t0 0x410:rt:t9:t0 0x414:rt:t0:t1 0x420:rt:t0:t1 0x47c:rt:t1:t2 0x480:rs:t1:t2 0x498:rd:t2:t3 0x4a0:rt:t2:t3 0x4a4:rt:t2:t3 0x4ac:rs:t2:t3 0x4b4:rt:t3:t4 0x4c0:rt:t3:t4 0x4c4:rt:t4:t5 0x4d0:rt:t4:t5 0x4d4:rt:t5:t6 0x4e0:rt:t5:t6 0x4e4:rt:t6:t7 0x4ec:rt:t6:t7 0x4f0:rt:t8:t9 0x4f4:rs:t8:t9 0x4f4:rt:t8:t9 0x4f8:rt:t7:t8 0x4fc:rs:t7:t8 0x4fc:rt:t7:t8 0x500:rd:t9:t0 0x500:rt:t8:t9 0x504:rd:t9:t0 0x504:rs:t9:t0 0x504:rt:t8:t9 0x508:rd:t0:t1 0x508:rs:t7:t8 0x508:rt:t9:t0 0x50c:rs:t0:t1 0x50c:rt:t1:t2 0x518:rt:t1:t2 */
#include "types.h"

extern u8 now_sub_sub_step;
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
extern void bunkasai_girl2_vst_bank_init(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

extern s16 now_serifu;
extern u8  now_year;
extern s32 girl2_flag;
extern u8  now_girl;
extern u8  now_sub_step;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkasai_girl2_vst_bk2_show_main(void) {
    s32 v0;
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
            bunkasai_basyo_init();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
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
            now_serifu = 41;
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        cd_read_start2(26, 0x801E8000, 18962);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(2);
            switch (v0) {
            case 0: case 1:
                now_serifu = 65;
                break;
            case 2:
                now_serifu = 66;
                break;
            case 3:
                now_serifu = 67;
                break;
            case 4:
                now_serifu = 68;
                break;

                break;
            }
            if (now_year == 97) {
                now_serifu = now_serifu + 4;
            }
            bunkasai_girl2_vst_bank_init();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        if ((s32)(girl2_flag << 29) >= 0) goto c10_42af0;
        if (now_year == 97) goto c10_42af0;
        v0 = get_g_zyotai_low(2);
        switch (v0) {
        case 0: case 1:
            now_serifu = 73;
            break;
        case 2:
            now_serifu = 74;
            break;
        case 3:
            now_serifu = 75;
            break;
        case 4:
            now_serifu = 76;
            break;

            break;
        }
        sub_sub_step_up();
        break;
c10_42af0:
        goto_sub_sub_step(12);
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        if (now_year == 97) {
            v0 = get_g_zyotai_low(2);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 79;
                break;
            case 2:
                now_serifu = 80;
                break;
            case 3:
                now_serifu = 81;
                break;
            case 4:
                now_serifu = 82;
                break;
            }
        } else {
            if (get_g_zyotai_low(2) == 4) {
                now_serifu = 78;
            } else {
                now_serifu = 77;
            }
        }
        sub_sub_step_up();
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        fade_out();
        break;
    case 15:
        bunka_sd_kind = 0;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 16:
        bustup_load_step(21038);
        break;
    case 17:
        if (get_CD_State2() & 1) {
            bunkasai_basyo_init();
            mes_base_y = 192;
            bustup_ptr_init();
            now_girl = 2;
            zyotai_sprite_set_chk();
            cd_read_start2(26, 0x801E8000, 18962);
        }
        break;
    case 18:
        if (get_g_zyotai_low(2) == 4) {
            fade_in_step();
            break;
        }
        fade_in();
        break;
    case 19:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(2);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 83;
                break;
            case 2:
                now_serifu = 85;
                break;
            case 3:
                now_serifu = 87;
                break;
            case 4:
                now_serifu = 89;
                break;
            }
            now_serifu = now_serifu + now_year * 7 - 665;
            bunkasai_girl2_vst_bank_init();
        }
        break;
    case 20:
        serifu_run();
        break;
    case 21:
        if (get_g_zyotai_low(2) == 4) {
            goto_sub_sub_step(23);
            break;
        }
        fade_down_main();
        break;
    case 22:
        serifu_run();
        break;
    case 23:
        fade_out_step();
        break;
    case 24:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 25:
        serifu_run();
        break;
    case 26:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
