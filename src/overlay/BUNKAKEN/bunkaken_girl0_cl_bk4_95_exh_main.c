/* @SymbolStart: 0x8014D13C */
/* @RodataBase: 0x801555A4 */
/* @SymbolSize: 1376 */
/* Address: 0x8014D144 */
/* Size: 1368 bytes (342 insns) */
/* @PostProc: patch_insn_reg_field 0x260:rt:t7:t8 0x264:rd:t6:t7 0x268:rs:t6:t7 0x280:rt:t8:t9 0x284:rs:t8:t9 0x284:rt:t8:t0 0x288:rt:t7:t8 0x290:rs:t8:t0 0x290:rt:t9:t1 0x298:rt:t9:t1 0x2d0:rt:t1:t3 0x2dc:rt:t2:t4 0x2e8:rt:t0:t2 0x2f4:rt:t0:t2 0x300:rt:t1:t3 0x308:rt:t2:t4 0x334:rt:t3:t5 0x33c:rt:t3:t5 0x344:rt:t4:t6 0x34c:rt:t4:t6 0x3f8:rt:t5:t7 0x400:rt:t5:t7 0x424:rt:t6:t8 0x42c:rt:t6:t8 0x4e0:rt:t7:t9 0x4e4:rs:t7:t9 0x4e4:rt:t7:t9 0x4ec:rs:t7:t9 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32  dec_bg_init(s32);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void zyotai_sprite_set_chk(void);
extern void bustup_face_change(s32);
extern void date_name_set(s32);
extern void serifu_run(void);
extern s32  get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void fade_out_step(void);
extern void memcpy(s32, s32, s32);
extern void bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);
extern void bunkaken_param_set2(void);

extern s32 bunka_girl2;
extern s32 kuchi_stop;
extern s16 now_serifu;
extern u8  now_girl;
extern s32 girl0_flag;
extern u8  girl0_flag_b;
extern u8 now_sub_sub_step;

void bunkaken_girl0_cl_bk4_95_exh_main(void)
{
    s32 idx = now_sub_sub_step;
    s32 ret;

    switch (idx) {
    case 0:
        value_cache_find_or_load(16655, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            ret = dec_bg_init(1);
            indexed_byte_lsb_set(1, ret);
            sub_sub_step_up();
        }
        break;
    case 3:
        bustup_load_step(22568);
        break;
    case 4:
        memcpy(0x80197000, 0x80180000, 0x16800);
        sub_sub_step_up();
        break;
    case 5:
        bustup_load_step(21668);
        break;
    case 6:
        memcpy(0x801AE000, 0x80180000, 0x16800);
        bustup_ptr_init();
        now_girl = 0;
        zyotai_sprite_set_chk();
        bunkaken_basyo_init3();
        cd_read_start2(23, 0x801E8000, 18923);
        break;
    case 7:
        fade_in_step();
        break;
    case 8:
        if (get_g_zyotai_low(0) == 4 || bunka_girl2 == 0) {
            sub_sub_step_up();
        } else {
            fade_up_main();
        }
        break;
    case 9:
        if (get_CD_State2() & 1) {
            bunkaken_param_set2();
            if (get_g_zyotai_low(0) == 4 || bunka_girl2 == 0) {
                now_serifu = 133;
                goto_sub_sub_step(30);
            } else {
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
                }
            }
        }
        break;
    case 10:
        serifu_run();
        break;
    case 11:
        {
            s32 t5 = girl0_flag << 29;
            if (t5 < 0) {
                goto_sub_sub_step(13);
            } else {
                u8 t0 = girl0_flag_b;
                now_serifu = 4;
                girl0_flag_b = t0 | 4;
                sub_sub_step_up();
            }
        }
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        switch (get_g_zyotai_low(0)) {
        case 0:
        case 1:
            now_serifu = 115;
            break;
        case 2:
            now_serifu = 121;
            break;
        case 3:
            now_serifu = 127;
            break;
        }
        sub_sub_step_up();
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        date_name_set(12);
        now_girl = 12;
        kuchi_stop = 1;
        sub_sub_step_up();
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        fade_down_main();
        break;
    case 18:
        memcpy(0x80180000, 0x80197000, 0x16800);
        sub_sub_step_up();
        break;
    case 19:
        kuchi_stop = 0;
        bustup_ptr_init();
        zyotai_sprite_set_chk();
        bustup_face_change(0);
        break;
    case 20:
        fade_up_main();
        break;
    case 21:
        serifu_run();
        break;
    case 22:
        date_name_set(0);
        now_girl = 0;
        kuchi_stop = 1;
        sub_sub_step_up();
        break;
    case 23:
        serifu_run();
        break;
    case 24:
        date_name_set(12);
        now_girl = 12;
        kuchi_stop = 0;
        sub_sub_step_up();
        break;
    case 25:
        serifu_run();
        break;
    case 26:
        fade_down_main();
        break;
    case 27:
        memcpy(0x80180000, 0x801AE000, 0x16800);
        sub_sub_step_up();
        break;
    case 28:
        bustup_ptr_init();
        now_girl = 0;
        zyotai_sprite_set_chk();
        date_name_set(0);
        break;
    case 29:
        fade_up_main();
        break;
    case 30:
        serifu_run();
        break;
    case 31:
        if (get_g_zyotai_low(0) == 4 || bunka_girl2 == 0) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 32:
        fade_out_step();
        break;
    case 33:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 34:
        serifu_run();
        break;
    case 35:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
