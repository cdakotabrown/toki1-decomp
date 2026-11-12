/* @SymbolStart: 0x8014BDC0 */
/* @RodataBase: 0x80155448 */
/* @SymbolSize: 1524 */
/* Address: 0x8014BDC8 */
/* Size: 1516 bytes (379 insns) */
/* @PostProc: patch_insn_reg_field 0x20c:rt:t0:t1 0x210:rd:t9:t0 0x214:rs:t9:t0 0x22c:rs:t0:t1 0x22c:rt:t0:t2 0x230:rt:t2:t4 0x234:rs:t0:t2 0x234:rt:t1:t3 0x238:rt:t1:t3 0x244:rt:t2:t4 0x27c:rt:t4:t6 0x288:rt:t5:t7 0x294:rt:t3:t5 0x2a0:rt:t3:t5 0x2ac:rt:t4:t6 0x2b4:rt:t5:t7 0x2c8:rt:t6:t8 0x2cc:rs:t6:t8 0x2cc:rt:t6:t8 0x2d4:rs:t6:t8 0x324:rt:t7:t9 0x328:rs:t7:t9 0x328:rt:t7:t9 0x32c:rt:t8:t0 0x330:rs:t8:t0 0x330:rt:t8:t0 0x338:rt:t7:t9 0x340:rs:t8:t0 0x340:rt:t9:t1 0x344:rt:t9:t1 0x38c:rt:t0:t2 0x390:rs:t0:t2 0x390:rt:t0:t2 0x398:rs:t0:t2 0x3c8:rt:t1:t3 0x3cc:rs:t1:t3 0x3f8:rt:t3:t5 0x404:rt:t4:t6 0x410:rt:t2:t4 0x41c:rt:t2:t4 0x428:rt:t3:t5 0x430:rt:t4:t6 0x444:rt:t5:t7 0x448:rs:t5:t7 0x448:rt:t5:t7 0x450:rs:t5:t7 0x454:rt:t6:t8 0x45c:rt:t6:t8 0x480:rt:t7:t9 0x484:rt:t8:t0 0x488:rs:t8:t0 0x488:rt:t8:t0 0x48c:rs:t7:t9 0x48c:rt:t7:t9 0x494:rd:t9:t1 0x494:rs:t7:t9 0x494:rt:t8:t0 0x49c:rt:t9:t1 0x4d4:rt:t1:t3 0x4e0:rt:t2:t4 0x4ec:rt:t0:t2 0x4f8:rt:t0:t2 0x504:rt:t1:t3 0x50c:rt:t2:t4 0x520:rt:t3:t5 0x524:rs:t3:t5 0x524:rt:t3:t5 0x52c:rs:t3:t5 0x588:rt:t4:t6 0x58c:rs:t4:t6 0x58c:rt:t4:t6 0x594:rs:t4:t6 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_sub_step(s32);
extern void goto_sub_sub_step(s32);
extern void device_cmd_send(s32);
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
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32  get_g_zyotai_low(s32);
extern void bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);
extern void bunkaken_param_set2(void);

extern s16 now_serifu;
extern u8  girl0_flag;
extern u8  now_girl;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern s32 bunka_girl2;
extern s32 bunka_aisyou;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;

void bunkaken_girl0_cl_bk3_95_fort_main(void) {
    s32 n;

    switch (now_sub_sub_step) {
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
            n = dec_bg_init(1);
            indexed_byte_lsb_set(1, n);
            bustup_cd_read(21578);
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
        if (get_g_zyotai_low(0) == 4 || bunka_girl2 == 0) {
            sub_sub_step_up();
        } else {
            fade_up_main();
        }
        break;
    case 6:
        if (get_CD_State2() & 1) {
            switch (get_g_zyotai_low(0)) {
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
                now_serifu = 86;
                break;
            }
            if (bunka_girl2 == 0) {
                now_serifu = 86;
            }
            bunkaken_param_set2();
        }
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        if ((*(s32 *)&girl0_flag << 29) < 0) {
            goto_sub_sub_step(10);
        } else {
            girl0_flag |= 4;
            now_serifu = 4;
            sub_sub_step_up();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        switch (get_g_zyotai_low(0)) {
        case 0:
        case 1:
            now_serifu = 68;
            break;
        case 2:
            now_serifu = 74;
            break;
        case 3:
            now_serifu = 80;
            break;
        }
        if (get_g_zyotai_low(0) == 4 || bunka_girl2 == 0) {
            goto_sub_sub_step(13);
        } else {
            sub_sub_step_up();
        }
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        fade_down_main();
        break;
    case 13:
        device_cmd_send(514);
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(11);
        break;
    case 14:
        cd_read_start2(23, 0x801E8000, 18923);
        sub_sub_step_up();
        break;
    case 15:
        if (get_g_zyotai_low(0) == 4 || bunka_girl2 == 0) {
            sub_sub_step_up();
        } else {
            fade_up_main();
        }
        break;
    case 16:
        if (get_CD_State2() & 1) {
            bunkaken_param_set2();
            switch (get_g_zyotai_low(0)) {
            case 0:
            case 1:
                now_serifu = 69;
                break;
            case 2:
                now_serifu = 75;
                break;
            case 3:
                now_serifu = 81;
                break;
            }
            if (get_g_zyotai_low(0) == 4 || bunka_girl2 == 0) {
                now_serifu = 87;
                goto_sub_sub_step(18);
            }
        }
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        now_serifu = now_serifu + bunka_aisyou;
        sub_sub_step_up();
        break;
    case 19:
        serifu_run();
        break;
    case 20:
        switch (get_g_zyotai_low(0)) {
        case 0:
        case 1:
            now_serifu = 73;
            break;
        case 2:
            now_serifu = 79;
            break;
        case 3:
            now_serifu = 85;
            break;
        }
        if (get_g_zyotai_low(0) == 4 || bunka_girl2 == 0) {
            goto_sub_sub_step(22);
        } else {
            sub_sub_step_up();
        }
        break;
    case 21:
        serifu_run();
        break;
    case 22:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 23:
        if (get_g_zyotai_low(0) == 4 || bunka_girl2 == 0) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 24:
        fade_out_step();
        break;
    case 25:
        serifu_run();
        break;
    case 26:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
