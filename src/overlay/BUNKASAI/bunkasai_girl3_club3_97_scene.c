/* @SymbolStart: 0x8013E59C */
/* Address: 0x8013E5A4 */
/* Size: 1588 bytes (397 insns) */
/* @SymbolSize: 1596 */
/* @RodataBase: 0x8015CFF8 */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=7 IDO52_AS1_XBB_SKIP_CAND=5,13,19,21 IDO52_FORCE_WEB_COLOR=16:v1 */
/* @PostProc: patch_insn_reg_field 0xd8:rt:t9:v1 0xe4:rt:t9:v1 0xe8:rt:t0:t9 0xf4:rt:t0:t9 0x14c:rt:t1:t0 0x150:rs:t1:t0 0x168:rd:t2:t1 0x170:rt:t2:t1 0x174:rt:t2:t1 0x17c:rs:t2:t1 0x184:rt:t3:v1 0x190:rt:t3:v1 0x194:rt:t4:v1 0x1a0:rt:t4:v1 0x1a4:rt:t5:v1 0x1b0:rt:t5:v1 0x1b4:rt:t6:v1 0x1bc:rt:t6:v1 0x20c:rt:t7:t2 0x210:rt:t9:t5 0x214:rd:t8:t4 0x214:rt:t7:t2 0x218:rs:t8:t4 0x230:rs:t9:t5 0x230:rt:t9:t5 0x238:rd:t0:t6 0x238:rt:t9:t5 0x23c:rt:t0:t6 0x240:rt:t1:v1 0x24c:rt:t1:v1 0x250:rt:t2:v1 0x254:rt:t2:v1 0x294:rt:t4:v1 0x2a0:rt:t5:v1 0x2ac:rt:t3:v1 0x2b8:rt:t3:v1 0x2c4:rt:t4:v1 0x2cc:rt:t5:v1 0x2d8:rt:t6:t7 0x2e4:rd:t7:t9 0x2e4:rt:t6:t7 0x2e8:rs:t7:t9 0x2ec:rt:t8:t1 0x2f4:rt:t8:t1 0x32c:rt:t9:t2 0x330:rs:t9:t2 0x330:rt:t9:t2 0x33c:rt:t9:t2 0x344:rt:t0:t3 0x348:rt:t0:t3 0x374:rt:t1:t4 0x380:rt:t1:t4 0x384:rt:t2:t5 0x390:rt:t2:t5 0x3ec:rt:t3:t6 0x3f0:rs:t3:t6 0x408:rd:t4:t7 0x410:rt:t4:t7 0x414:rt:t4:t7 0x41c:rs:t4:t7 0x424:rt:t5:v1 0x430:rt:t5:v1 0x434:rt:t6:v1 0x440:rt:t6:v1 0x444:rt:t7:v1 0x450:rt:t7:v1 0x454:rt:t8:v1 0x45c:rt:t8:v1 0x490:rt:t9:t8 0x4a4:rs:t9:t8 0x4a4:rt:t9:t8 0x4ac:rs:t9:t8 0x4ac:rt:t0:t9 0x4b0:rs:t0:t9 0x4d4:rt:t2:v1 0x4e0:rt:t3:v1 0x4ec:rt:t1:v1 0x4f8:rt:t1:v1 0x504:rt:t2:v1 0x510:rt:t3:v1 0x564:rt:t4:t0 0x56c:rd:t5:t2 0x56c:rt:t4:t0 0x570:rs:t5:t2 0x578:rt:t6:t4 0x580:rd:t8:t6 0x584:rs:t6:t4 0x584:rt:t7:t5 0x588:rt:t7:t5 0x58c:rt:t8:t6 0x594:rt:t0:t8 0x59c:rd:t1:t9 0x59c:rt:t0:t8 0x5a0:rs:t1:t9 0x5a4:rt:t2:t1 0x5ac:rt:t2:t1 */
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
extern void bunkasai_girl3_clubs2_3_srf_bank_set(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

extern s16 now_serifu;
extern volatile u8 screen_w[];
extern s16 girl3_syoushin;
extern u8  girl3_basyo;
extern u8  now_girl;
extern u8  now_sub_step;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkasai_girl3_club3_97_scene(void) {
    s32 v0;
    s32 msg;
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
            bunkasai_basyo_init();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            bustup_cd_read(22703);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            now_serifu = 41;
            bustup_ptr_init();
            now_girl = 3;
            zyotai_sprite_set_chk();
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        cd_read_start2(23, 0x801E8000, 19158);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(3);
            switch (v0) {
            case 0:
            case 1:
                msg = 1;
                now_serifu = msg;
                break;
            case 2:
                msg = 2;
                now_serifu = msg;
                break;
            case 3:
                msg = 3;
                now_serifu = msg;
                break;
            case 4:
                msg = 12;
                now_serifu = msg;
                break;
            }
            bunkasai_girl3_clubs2_3_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        if (get_g_zyotai_low(3) == 4U) {
            goto_sub_sub_step(14);
            break;
        }
        if ((*(volatile s32 *) &screen_w[624] << 29) >= 0) {
            goto_sub_sub_step(12);
            break;
        }
        if (((u32) girl3_basyo >> 4) == 4U) {
            msg = 4;
            now_serifu = msg;
        } else {
            msg = 5;
            now_serifu = msg;
        }
        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        switch (get_g_zyotai_low(3)) {
        case 0:
        case 1:
            now_serifu = 6;
            break;
        case 2:
            now_serifu = 7;
            break;
        case 3:
            now_serifu = 8;
            break;
        }
        msg = now_serifu;
        if ((*(volatile s32 *) &screen_w[624] << 29) >= 0) {
            now_serifu = msg + 3;
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
        bustup_load_step(22703);
        break;
    case 17:
        bunkasai_basyo_init();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 3;
        zyotai_sprite_set_chk();
        cd_read_start2(23, 0x801E8000, 19158);
        break;
    case 18:
        if (get_g_zyotai_low(3) == 4U) {
            fade_in_step();
        } else {
            fade_in();
        }
        break;
    case 19:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(3);
            switch (v0) {
            case 0:
            case 1:
                msg = 178;
                now_serifu = msg;
                break;
            case 2:
                msg = 179;
                now_serifu = msg;
                break;
            case 3:
                msg = 180;
                now_serifu = msg;
                break;
            case 4:
                msg = 13;
                now_serifu = msg;
                break;
            }
            bunkasai_girl3_clubs2_3_srf_bank_set();
        }
        break;
    case 20:
        serifu_run();
        break;
    case 21:
        if (get_g_zyotai_low(3) == 4U) {
            goto_sub_sub_step(25);
            break;
        }
        if ((girl3_syoushin & 3) == 0) {
            switch (get_g_zyotai_low(3)) {
            case 0:
            case 1:
                now_serifu = 23;
                break;
            case 2:
                now_serifu = 25;
                break;
            case 3:
                now_serifu = 27;
                break;
            }
        } else {
            v0 = get_g_zyotai_low(3);
            switch (v0) {
            case 0:
            case 1:
                msg = 77;
                break;
            case 2:
                msg = 79;
                break;
            case 3:
                msg = 81;
                break;
            default:
                msg = now_serifu;
                break;
            }
            if ((*(volatile s32 *) &screen_w[624] << 29) >= 0) {
                screen_w[624] = screen_w[624] | 4;
                msg = (s16)(msg + 6);
            }
            now_serifu = msg;
            if (((u32) screen_w[626] >> 4) == 5) {
                now_serifu = msg + 12;
            }
        }
        sub_sub_step_up();
        break;
    case 22:
        serifu_run();
        break;
    case 23:
        fade_down_main();
        break;
    case 24:
        serifu_run();
        break;
    case 25:
        fade_out_step();
        break;
    case 26:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 27:
        serifu_run();
        break;
    case 28:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
