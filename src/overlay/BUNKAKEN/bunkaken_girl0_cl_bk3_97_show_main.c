/* @SymbolStart: 0x8014CB8C */
/* Address: 0x8014CB94 */
/* Size: 1448 bytes (362 insns) */
/* @SymbolSize: 1456 */
/* @RodataBase: 0x80155544 */
/* @PostProc: patch_insn_reg_field 0x208:rt:v0:t5 0x20c:rs:v0:t5 0x20c:rt:v0:t5 0x210:rt:t6:t8 0x214:rd:t5:t7 0x214:rt:v0:t5 0x218:rs:t5:t7 0x230:rt:v0:t9 0x234:rs:v0:t9 0x234:rt:v0:t0 0x238:rt:t6:t8 0x240:rs:v0:t0 0x240:rt:t7:t1 0x248:rt:t7:t1 0x280:rt:t9:t3 0x28c:rt:t0:t4 0x298:rt:t8:t2 0x2a4:rt:t8:t2 0x2b0:rt:t9:t3 0x2b8:rt:t0:t4 0x2cc:rt:t1:t5 0x2d0:rs:t1:t5 0x2d0:rt:t1:t5 0x2d8:rs:t1:t5 0x2dc:rt:t2:t6 0x2e4:rt:t2:t6 0x318:rt:t3:t7 0x31c:rs:t3:t7 0x31c:rt:t3:t7 0x324:rs:t3:t7 0x35c:rt:t7:t1 0x360:rt:t4:t8 0x364:rs:t4:t8 0x364:rt:t4:t8 0x368:rt:t6:t0 0x36c:rs:t4:t8 0x374:rt:t5:t9 0x380:rt:t5:t9 0x384:rt:t6:t0 0x388:rs:t7:t1 0x388:rt:t7:t1 0x38c:rt:t8:t2 0x390:rs:t8:t2 0x390:rt:t8:t2 0x398:rt:t7:t1 0x3a0:rs:t8:t2 0x3a0:rt:t9:t3 0x3a4:rt:t9:t3 0x3d8:rt:t0:t4 0x3dc:rs:t0:t4 0x3ec:rt:t1:t5 0x3f8:rt:t1:t5 0x434:rt:t2:t6 0x438:rs:t2:t6 0x438:rt:t2:t6 0x440:rs:t2:t6 0x470:rt:t3:t7 0x474:rs:t3:t7 0x498:rt:t5:t9 0x4a4:rt:t6:t0 0x4b0:rt:t4:t8 0x4bc:rt:t4:t8 0x4c8:rt:t5:t9 0x4d0:rt:t6:t0 0x4e4:rt:t7:t1 0x4e8:rs:t7:t1 0x4e8:rt:t7:t1 0x4f0:rs:t7:t1 0x4f4:rt:t8:t2 0x4fc:rt:t8:t2 0x530:rt:t9:t3 0x534:rs:t9:t3 0x534:rt:t9:t3 0x53c:rs:t9:t3 */
#include "types.h"

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
extern void bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);
extern void bunkaken_param_set2(void);

extern s32 bunka_girl2;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;
extern s16 now_serifu;
extern u8  now_girl;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern union { s32 w; volatile u8 b; } girl0_flag;

void bunkaken_girl0_cl_bk3_97_show_main(void)
{
    s32 idx = now_sub_sub_step;
    s32 v0;
    s32 t9;
    s32 ret;

    switch (idx) {
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
            ret = dec_bg_init(1);
            indexed_byte_lsb_set(1, ret);
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
        v0 = get_g_zyotai_low(0);
        t9 = bunka_girl2;
        if (v0 == 4 || t9 == 0) {
            sub_sub_step_up();
        } else {
            fade_up_main();
        }
        break;
    case 6:
        if (get_CD_State2() & 1) {
            bunkaken_param_set2();
            v0 = get_g_zyotai_low(0);
            switch (v0) {
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
            }
            v0 = get_g_zyotai_low(0);
            if (v0 == 4 || bunka_girl2 == 0) {
                goto_sub_sub_step(10);
            }
        }
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        {
            s32 t8 = girl0_flag.w;
            s32 t1 = 4;
            s32 t0 = t8 << 29;
            if (t0 < 0) {
                goto_sub_sub_step(10);
            } else {
                u8 t3 = girl0_flag.b;
                now_serifu = (s16)t1;
                girl0_flag.b = t3 | (u8)t1;
                sub_sub_step_up();
            }
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(0);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 107;
            break;
        case 2:
            now_serifu = 108;
            break;
        case 3:
            now_serifu = 109;
            break;
        }
        v0 = get_g_zyotai_low(0);
        if (v0 == 4 || bunka_girl2 == 0) {
            now_serifu = 113;
        }
        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        v0 = get_g_zyotai_low(0);
        t9 = bunka_girl2;
        if (v0 == 4 || t9 == 0) {
            fade_out_step();
        } else {
            fade_out();
        }
        break;
    case 13:
        v0 = get_g_zyotai_low(0);
        t9 = bunka_girl2;
        if (v0 == 4 || t9 == 0) {
            bunka_sd_kind = 1;
        } else {
            bunka_sd_kind = 2;
        }
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 14:
        bustup_cd_read(21578);
        sub_sub_step_up();
        break;
    case 15:
        if (get_CD_State2() & 1) {
            bunkaken_basyo_init3();
            mes_base_y = 192;
            bustup_ptr_init();
            now_girl = 0;
            zyotai_sprite_set_chk();
            cd_read_start2(23, 0x801E8000, 18923);
        }
        break;
    case 16:
        v0 = get_g_zyotai_low(0);
        t9 = bunka_girl2;
        if (v0 == 4 || t9 == 0) {
            fade_in_step();
        } else {
            fade_in();
        }
        break;
    case 17:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(0);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 110;
                break;
            case 2:
                now_serifu = 111;
                break;
            case 3:
                now_serifu = 112;
                break;
            }
            v0 = get_g_zyotai_low(0);
            if (v0 == 4 || bunka_girl2 == 0) {
                now_serifu = 114;
            }
            bunkaken_param_set2();
        }
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        v0 = get_g_zyotai_low(0);
        t9 = bunka_girl2;
        if (v0 == 4 || t9 == 0) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 20:
        fade_out_step();
        break;
    case 21:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 22:
        serifu_run();
        break;
    case 23:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
