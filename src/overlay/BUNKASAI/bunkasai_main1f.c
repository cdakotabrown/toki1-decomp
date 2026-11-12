/* @SymbolStart: 0x80154D64 */
/* @RodataBase: 0x8015F880 */
/* @SymbolSize: 1572 */
/* @PostProc: patch_insn_reg_field 0xc8:rt:t0:v0 0xcc:rt:t9:v0 0xd8:rt:t9:v0 0xe4:rt:t0:v0 0x108:rt:t1:t9 0x10c:rs:t1:t9 0x10c:rt:t1:t9 0x114:rt:t1:t9 0x11c:rt:t2:t0 0x120:rt:t2:t0 0x168:rt:t3:t1 0x16c:rs:t3:t1 0x1a0:rt:t4:t2 0x1ac:rt:t4:t2 0x1b0:rt:t5:t3 0x1bc:rt:t5:t3 0x1f4:rt:t6:t4 0x1f8:rs:t6:t4 0x210:rd:t7:t5 0x218:rt:t7:t5 0x21c:rt:t7:t5 0x224:rs:t7:t5 0x22c:rt:t8:v0 0x238:rt:t8:v0 0x23c:rt:t9:v0 0x248:rt:t9:v0 0x24c:rt:t0:v0 0x258:rt:t0:v0 0x25c:rt:t1:v0 0x264:rt:t1:v0 0x298:rd:t2:t6 0x2a0:rt:t2:t6 0x2a4:rt:t2:t6 0x2ac:rs:t2:t6 0x2b4:rt:t3:v0 0x2c0:rt:t3:v0 0x2c4:rt:t4:v0 0x2cc:rt:t4:v0 0x308:rd:t5:t7 0x310:rt:t5:t7 0x314:rt:t5:t7 0x31c:rs:t5:t7 0x324:rt:t6:v0 0x328:rs:t6:v0 0x328:rt:t6:v0 0x330:rd:t7:t8 0x330:rt:t6:v0 0x334:rd:t7:t8 0x334:rs:t7:t8 0x334:rt:t6:v0 0x338:rs:t7:t8 0x338:rt:t8:t0 0x340:rt:t8:t0 0x344:rt:t9:v0 0x348:rs:t9:v0 0x348:rt:t9:v0 0x350:rd:t0:t1 0x350:rt:t9:v0 0x354:rd:t0:t1 0x354:rs:t0:t1 0x354:rt:t9:v0 0x358:rs:t0:t1 0x358:rt:t1:t3 0x360:rt:t1:t3 0x364:rt:t2:v0 0x368:rs:t2:v0 0x368:rt:t2:v0 0x370:rd:t3:t4 0x370:rt:t2:v0 0x374:rd:t3:t4 0x374:rs:t3:t4 0x374:rt:t2:v0 0x378:rs:t3:t4 0x378:rt:t4:t6 0x380:rt:t4:t6 0x3b0:rt:t5:t7 0x3f0:rs:t5:t7 0x3f0:rt:t5:t7 0x3f8:rs:t5:t7 0x3f8:rt:t6:t8 0x3fc:rs:t6:t8 0x400:rt:t8:t1 0x404:rt:t7:t0 0x410:rt:t7:t0 0x414:rs:t8:t1 0x414:rt:t8:t1 0x420:rs:t8:t1 0x424:rt:t9:t3 0x42c:rt:t9:t3 0x450:rt:t0:t4 0x454:rs:t0:t4 0x454:rt:t0:t4 0x45c:rs:t0:t4 0x45c:rt:t1:t5 0x460:rs:t1:t5 0x484:rt:t2:t6 0x4c4:rs:t2:t6 0x4c4:rt:t2:t6 0x4cc:rd:t3:t8 0x4cc:rt:t2:t6 0x4d0:rs:t3:t8 0x4d4:rt:t6:t1 0x4dc:rd:t4:t9 0x4e0:rt:t4:t9 0x4e4:rs:t6:t1 0x4e4:rt:t6:t1 0x4f0:rs:t6:t1 0x4f4:rt:t8:t4 0x4f8:rt:t7:t3 0x500:rt:t7:t3 0x504:rs:t8:t4 0x504:rt:t8:t5 0x50c:rs:t8:t5 0x50c:rt:t9:t6 0x514:rt:t9:t6 0x5a4:rd:t0:t7 0x5ac:rt:t0:t7 0x5b0:rt:t0:t7 0x5b8:rs:t0:t7 0x5c0:rt:t1:v0 0x5cc:rt:t1:v0 0x5d0:rt:t2:v0 0x5dc:rt:t2:v0 0x5e0:rt:t3:v0 0x5e8:rt:t3:v0 */
/* Address: 0x80154D6C */
/* Size: 1564 bytes (391 insns) */
#include "types.h"

extern u8 now_sub_sub_step;
extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern void goto_sub_sub_step(s32);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void set_dec_bri(s32);
extern void dec_bg_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32  dec_bg_init(s32);
extern void bustup_ptr_init(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32  get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init2(void);
extern void bunkasai_girl1_clubs0_2_srf_bank_set(void);

extern s16 now_serifu;
extern u8  now_year;
extern s16 girl1_syoushin;
extern union {
    s32 w;
    u8  b;
} girl1_flag;
extern u8  now_girl;
extern u8  now_sub_step;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkasai_main1f(void) {
    s32 v0;
    s16 n;
    switch (now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16628, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkasai_basyo_init2();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        if (now_year == 95) {
            now_serifu = 39;
        } else {
            now_serifu = 40;
        }
        break;
    case 4:
        serifu_run();
        break;
    case 5:
        fade_out_step();
        break;
    case 6:
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 7:
        dec_bg_reset();
        set_dec_bri(0);
        value_cache_find_or_load(16628, 1);
        sub_sub_step_up();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            bustup_load_step(23873);
        }
        break;
    case 9:
        v0 = dec_bg_init(1);
        indexed_byte_lsb_set(1, v0);
        bunkasai_basyo_init2();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 1;
        zyotai_sprite_set_chk();
        cd_read_start2(20, 0x801E8000, 19075);
        break;
    case 10:
        fade_in();
        break;
    case 11:
        if (get_CD_State2() & 1) {
            switch (get_g_zyotai_low(1)) {
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
                now_serifu = 4;
                break;
            }
            bunkasai_girl1_clubs0_2_srf_bank_set();
        }
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        switch (get_g_zyotai_low(1)) {
        case 0:
        case 1:
        case 2:
        case 3:
            now_serifu = 7;
            break;
        case 4:
            now_serifu = 8;
            break;
        }
        sub_sub_step_up();
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        sub_sub_step_up();
        switch (get_g_zyotai_low(1)) {
        case 0:
        case 1:
            now_serifu = now_year * 3 - 232;
            break;
        case 2:
            now_serifu = now_year * 3 - 231;
            break;
        case 3:
            now_serifu = now_year * 3 - 230;
            break;
        case 4:
            goto_sub_sub_step(26);
            break;
        }
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        switch (get_g_zyotai_low(1)) {
        case 0:
        case 1:
            n = 62;
            break;
        case 2:
            n = 63;
            break;
        case 3:
            n = 64;
            break;
        default:
            n = now_serifu;
            break;
        }
        if ((girl1_syoushin & 3) == 0) {
            now_serifu = n + 6;
        } else {
            now_serifu = n;
            if (now_year == 97) {
                now_serifu = n + 3;
            }
        }
        sub_sub_step_up();
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        if ((girl1_syoushin & 3) == 0) {
            goto_sub_sub_step(26);
            break;
        }
        switch (get_g_zyotai_low(1)) {
        case 0:
        case 1:
            n = 71;
            break;
        case 2:
            n = 72;
            break;
        case 3:
            n = 73;
            break;
        default:
            n = now_serifu;
            break;
        }
        if ((girl1_flag.w << 29) >= 0) {
            n = n + 3;
        }
        now_serifu = n;
        if (now_year == 97) {
            now_serifu = n + 6;
        }
        girl1_flag.b |= 4;
        sub_sub_step_up();
        break;
    case 20:
        serifu_run();
        break;
    case 21:
        fade_down_main();
        break;
    case 22:
        fade_out_step();
        break;
    case 23:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 24:
        serifu_run();
        break;
    case 25:
        bunkasai_sub_reset();
        break;
    case 26:
        fade_down_main();
        break;
    case 27:
        switch (get_g_zyotai_low(1)) {
        case 0:
        case 1:
            now_serifu = 13;
            break;
        case 2:
        case 3:
            now_serifu = 12;
            break;
        case 4:
            now_serifu = 14;
            break;
        }
        sub_sub_step_up();
        break;
    case 28:
        serifu_run();
        break;
    case 29:
        goto_sub_sub_step(22);
        break;
    default:
        game_reset();
        break;
    }
}
