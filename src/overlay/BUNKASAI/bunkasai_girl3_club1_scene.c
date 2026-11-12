/* @SymbolStart: 0x8013CCAC */
/* @RodataBase: 0x8015CD4C */
/* @CompileEnv: IDO52_UGEN_NUM_TREGS=5 */
/* Address: 0x8013CCB4 */
/* Size: 1352 bytes (338 insns) */
/* @SymbolSize: 1360 */
/* (5) `@CompileEnv: IDO52_UGEN_NUM_TREGS=5` -- required for case 15's tail */
/* @PostProc: patch_insn_reg_field 0xc8:rt:t0:v0 0xcc:rt:t9:v0 0xd8:rt:t9:v0 0xe4:rt:t0:v0 0x108:rt:t1:t9 0x10c:rs:t1:t9 0x10c:rt:t1:t9 0x114:rt:t1:t9 0x11c:rt:t2:t0 0x120:rt:t2:t0 0x168:rt:t3:t1 0x16c:rs:t3:t1 0x18c:rt:t4:t2 0x190:rs:t4:t2 0x1b4:rt:t6:t3 0x1c0:rt:t6:t3 0x1c4:rt:t7:t4 0x1d0:rt:t7:t4 0x208:rt:t8:t5 0x20c:rs:t8:t5 0x224:rd:t9:t6 0x22c:rt:t9:t6 0x230:rt:t9:t6 0x238:rs:t9:t6 0x240:rt:t0:v0 0x24c:rt:t0:v0 0x250:rt:t1:v0 0x25c:rt:t1:v0 0x260:rt:t2:v0 0x26c:rt:t2:v0 0x270:rt:t3:v0 0x278:rt:t3:v0 0x2b4:rd:t4:t7 0x2bc:rt:t4:t7 0x2c0:rt:t4:t7 0x2c8:rs:t4:t7 0x2d0:rt:t6:v0 0x2d4:rs:t6:v0 0x2d4:rt:t6:v0 0x2dc:rd:t7:t8 0x2dc:rt:t6:v0 0x2e0:rd:t7:t8 0x2e0:rs:t7:t8 0x2e0:rt:t6:v0 0x2e4:rs:t7:t8 0x2e4:rt:t8:t0 0x2ec:rt:t8:t0 0x2f0:rt:t9:v0 0x2f4:rs:t9:v0 0x2f4:rt:t9:v0 0x2fc:rd:t0:t1 0x2fc:rt:t9:v0 0x300:rd:t0:t1 0x300:rs:t0:t1 0x300:rt:t9:v0 0x304:rs:t0:t1 0x304:rt:t1:t3 0x30c:rt:t1:t3 0x310:rt:t2:v0 0x314:rs:t2:v0 0x314:rt:t2:v0 0x31c:rd:t3:t4 0x31c:rt:t2:v0 0x320:rd:t3:t4 0x320:rs:t3:t4 0x320:rt:t2:v0 0x324:rs:t3:t4 0x324:rt:t4:t6 0x32c:rt:t4:t6 0x360:rt:t6:t7 0x374:rs:t6:t7 0x374:rt:t6:t7 0x37c:rs:t6:t7 0x380:rt:t7:t8 0x394:rs:t7:t8 0x394:rt:t7:t8 0x39c:rs:t7:t8 0x39c:rt:t8:t9 0x3a0:rs:t8:t9 0x414:rt:t9:t0 0x454:rs:t9:t0 0x454:rt:t9:t0 0x458:rt:t3:t5 0x45c:rd:t0:t2 0x45c:rt:t9:t0 0x460:rs:t0:t2 0x464:rt:t6:t8 0x468:rs:t3:t5 0x468:rt:t3:t6 0x470:rd:t1:t3 0x478:rs:t3:t6 0x478:rt:t4:t7 0x47c:rt:t1:t3 0x480:rt:t4:t7 0x484:rs:t6:t8 0x484:rt:t6:t8 0x48c:rd:t7:t9 0x48c:rt:t6:t8 0x490:rs:t7:t9 0x494:rt:t0:t2 0x49c:rd:t8:t0 0x4a0:rt:t8:t0 0x4a8:rs:t0:t2 0x4a8:rt:t0:t2 0x4b4:rs:t0:t2 0x4b8:rt:t1:t4 0x4c0:rt:t1:t4 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern void goto_sub_step(s32);
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
extern void bunkasai_girl3_clubs0_1_srf_bank_set(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init2(void);

extern volatile s16 now_serifu;
extern u8  now_year;
extern s16 girl3_syoushin;
extern u8  girl3_flag;
extern u8  girl3_basyo;
extern u8  now_girl;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkasai_girl3_club1_scene(void) {
    s32 idx = now_sub_sub_step;
    s32 v0;
    s32 n;
    s32 sel;

    switch (idx) {
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
            n = 39;
            now_serifu = n;
        } else {
            n = 40;
            now_serifu = n;
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
            bustup_load_step(22703);
        }
        break;
    case 9:
        if (get_CD_State2() & 1) {
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            bunkasai_basyo_init2();
            mes_base_y = 192;
            bustup_ptr_init();
            now_girl = 3;
            zyotai_sprite_set_chk();
            cd_read_start2(25, 0x801E8000, 19133);
        }
        break;
    case 10:
        fade_in();
        break;
    case 11:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(3);
            switch (v0) {
            case 0:
            case 1:
                n = 110;
                now_serifu = n;
                break;
            case 2:
                n = 111;
                now_serifu = n;
                break;
            case 3:
                n = 112;
                now_serifu = n;
                break;
            case 4:
                n = 122;
                now_serifu = n;
                break;
            }
            bunkasai_girl3_clubs0_1_srf_bank_set();
        }
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        sub_sub_step_up();
        v0 = get_g_zyotai_low(3);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = now_year * 3 - 172;
            break;
        case 2:
            now_serifu = now_year * 3 - 171;
            break;
        case 3:
            now_serifu = now_year * 3 - 170;
            break;
        case 4:
            goto_sub_sub_step(17);
            break;
        }
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        v0 = get_g_zyotai_low(3);
        if (v0 == 4) {
            goto_sub_sub_step(17);
        } else if (now_year == 97) {
            goto_sub_step(79);
        } else {
            if ((girl3_syoushin & 3) == 0) {
                sel = get_g_zyotai_low(3);
                switch (sel) {
                case 0:
                case 1:
                    sel = 104;
                    goto stA;
                case 2:
                    sel = 106;
                    goto stB;
                case 3:
                    sel = 108;
                    goto stC;
                default:
                    goto stD;
                stA:
                    now_serifu = sel;
                    goto stD;
                stB:
                    now_serifu = sel;
                    goto stD;
                stC:
                    now_serifu = sel;
                    goto stD;
                stD:
                    ;
                }
            } else {
                v0 = get_g_zyotai_low(3);
                switch (v0) {
                case 0:
                case 1:
                    n = 124;
                    break;
                case 2:
                    n = 126;
                    break;
                case 3:
                    n = 128;
                    break;
                default:
                    n = now_serifu;
                    break;
                }
                if ((s32)(*(s32 *)&girl3_flag << 29) >= 0) {
                    n = (s16) (n + 6);
                    girl3_flag |= 4;
                }
                if (((u32) girl3_basyo >> 4) == 5) {
                    n = (s16) (n + 12);
                }
                now_serifu = n;
                if (now_year == 96) {
                    now_serifu = n + 24;
                }
            }
            sub_sub_step_up();
        }
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        fade_down_main();
        break;
    case 18:
        serifu_run();
        break;
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
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
