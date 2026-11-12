/* @SymbolStart: 0x8013C5A8 */
/* @SymbolSize: 1216 */
/* @RodataBase: 0x8015CC88 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=54:v1,34:v1 */
/* @PostProc: patch_insn_reg_field 0x104:rt:t2:v1 0x108:rt:t1:v1 0x114:rt:t1:v1 0x120:rt:t2:v1 0x170:rt:t3:t1 0x174:rs:t3:t1 0x18c:rd:t4:t2 0x194:rt:t4:t2 0x198:rt:t4:t2 0x1a0:rs:t4:t2 0x1a8:rt:t5:v1 0x1b4:rt:t5:v1 0x1b8:rt:t6:v1 0x1c4:rt:t6:v1 0x1c8:rt:t7:v1 0x1d4:rt:t7:v1 0x1d8:rt:t8:v1 0x1e0:rt:t8:v1 0x214:rd:t9:t3 0x21c:rt:t9:t3 0x220:rt:t9:t3 0x228:rs:t9:t3 0x230:rt:t0:v1 0x23c:rt:t0:v1 0x240:rt:t1:v1 0x24c:rt:t1:v1 0x250:rt:t2:v1 0x25c:rt:t2:v1 0x260:rt:t3:v1 0x268:rt:t3:v1 0x284:rt:t5:t6 0x28c:rt:t5:t6 0x2e0:rt:t6:t7 0x320:rs:t6:t7 0x320:rt:t6:t7 0x324:rt:t8:t0 0x328:rs:t6:t7 0x328:rt:t7:t8 0x32c:rs:t7:t8 0x330:rt:t9:t1 0x33c:rt:t8:t0 0x340:rs:t9:t1 0x340:rt:t9:t1 0x348:rs:t9:t1 0x34c:rt:t0:v0 0x350:rs:t0:v0 0x350:rt:t0:v0 0x354:rs:t0:v0 0x354:rt:t1:t2 0x35c:rd:t2:t3 0x35c:rt:t1:t2 0x360:rs:t2:t3 0x364:rt:t3:t5 0x370:rt:t3:t5 0x374:rt:t4:v0 0x378:rs:t4:v0 0x378:rt:t4:v0 0x37c:rs:t4:v0 0x37c:rt:t5:t6 0x384:rd:t6:t7 0x384:rt:t5:t6 0x388:rs:t6:t7 0x38c:rt:t9:t0 0x394:rd:t7:t8 0x398:rt:t7:t8 0x39c:rs:t9:t0 0x39c:rt:t9:t0 0x3a4:rd:t0:t2 0x3a4:rt:t9:t0 0x3a8:rs:t0:t2 0x3b0:rt:t1:t4 0x3b8:rt:t1:t4 0x464:rt:t2:t5 0x468:rs:t2:t5 0x468:rt:t2:t5 0x470:rs:t2:t5 0x480:rt:t3:t7 0x488:rs:t3:t7 0x488:rt:t4:t8 0x490:rt:t4:t8 0x498:rt:t5:t0 0x4a0:rs:t5:t0 0x4a0:rt:t6:t1 0x4a4:rt:t6:t1 */
/* Address: 0x8013C5B0 */
/* Size: 1208 bytes (302 insns) */
#include "types.h"

extern u8 screen_w[];

extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32 a0);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32 a0, s32 a1);
extern s32  value_cache_find_or_load(s32 a0, s32 a1);
extern s32  dec_bg_init(s32 a0);
extern void bustup_cd_read(s32 a0);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32  get_g_zyotai_low(s32 a0);
extern void bunkasai_girl3_clubs0_1_srf_bank_set(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

extern s16 now_serifu;
extern u8  now_year;
extern s16 girl3_syoushin;
extern s32 girl3_flag;
extern u8  now_girl;

extern u8 now_sub_sub_step;
void bunkasai_step42(void) {
    s32 dispatch = now_sub_sub_step;

    s32 v0;
    s32 v1;
    u8 *p;
    s32 t;

    switch ((u32)dispatch) {
    case 0:
        value_cache_find_or_load(16618, 1);
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
            bustup_ptr_init();
            now_girl = 3;
            zyotai_sprite_set_chk();
            if (now_year == 95) {
                now_serifu = 36;
            } else {
                now_serifu = 37;
            }
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        cd_read_start2(25, 0x801E8000, 19133);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(3);
            switch ((u32)v0) {
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
            bunkasai_girl3_clubs0_1_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(3);
        switch ((u32)v0) {
        case 0:
        case 1:
            now_serifu = 5;
            break;
        case 2:
            now_serifu = 6;
            break;
        case 3:
            now_serifu = 7;
            break;
        case 4:
            now_serifu = 8;
            break;
        }
        t = now_serifu;
        if (now_year == 96) {
            now_serifu = t + 5;
        }

        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        v0 = get_g_zyotai_low(3);
        if (v0 == 4) {
            fade_down_main();
            break;
        }
        v0 = get_g_zyotai_low(3);
        switch ((u32)v0) {
        case 0:
        case 1:
            v1 = 15;
            break;
        case 2:
            v1 = 17;
            break;
        case 3:
            v1 = 19;
            break;
        default:
            v1 = now_serifu;
            break;
        }
        if ((girl3_syoushin & 3) == 0) {
            now_serifu = v1 + 24;
            goto c12_tail;
        }
        if (now_year == 96) {
            p = (u8 *)((u32)screen_w | 0);
            if (((u32)p[626] >> 4) == 4) {
                now_serifu = v1 + 30;
                goto c12_tail;
            }
        }
        p = (u8 *)((u32)screen_w | 0);
        if (((u32)p[626] >> 4) == 5) {
            v1 = (s16)(v1 + 12);
        }
        now_serifu = v1;
        if ((girl3_flag << 29) >= 0) {
            now_serifu = v1 + 6;
        }
    c12_tail:
        sub_sub_step_up();
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        v0 = get_g_zyotai_low(3);
        if ((u32)v0 < 4) {
            fade_down_main();
        } else {
            goto_sub_sub_step(16);
        }
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        fade_out_step();
        break;
    case 17:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        v0 = get_g_zyotai_low(3);
        if ((u32)v0 < 4) {
            if (now_year == 95) {
                p = (u8 *)((u32)screen_w | 0);
                p[625] |= 0x10;
            } else {
                p = (u8 *)((u32)screen_w | 0);
                p[625] |= 0x20;
            }
        }
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
