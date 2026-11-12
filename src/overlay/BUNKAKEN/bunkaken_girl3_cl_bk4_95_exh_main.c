/* @SymbolStart: 0x80142A50 */
/* @RodataBase: 0x801547A0 */
/* @PostProc: patch_insn_reg_field 0x24c:rt:v0:t6 0x250:rs:v0:t6 0x250:rt:v0:t6 0x254:rt:t7:t9 0x258:rd:t6:t8 0x258:rt:v0:t6 0x25c:rs:t6:t8 0x274:rt:t8:t0 0x278:rs:t8:t0 0x278:rt:t8:t1 0x27c:rt:t7:t9 0x284:rs:t8:t1 0x284:rt:t9:t2 0x28c:rt:t9:t2 0x2c4:rt:t1:t4 0x2d0:rt:t2:t5 0x2dc:rt:t0:t3 0x2e8:rt:t0:t3 0x2f4:rt:t1:t4 0x2fc:rt:t2:t5 0x318:rt:t3:t6 0x31c:rs:t3:t6 0x31c:rt:t3:t6 0x324:rs:t3:t6 0x354:rt:t4:t7 0x35c:rt:t4:t7 0x364:rt:t5:t8 0x36c:rt:t5:t8 0x484:rt:t6:t9 0x490:rt:t6:t9 0x4cc:rt:t7:t0 0x4d4:rt:t7:t0 0x4dc:rt:t8:t1 0x4e4:rt:t8:t1 0x508:rt:t9:t2 0x510:rt:t9:t2 */
/* @SymbolSize: 1428 */
/* Address: 0x80142A58 */
/* Size: 1428 bytes (357 insns) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 goto_sub_sub_step(s32);
extern s32 get_CD_State2(void);
extern s32 cd_read_start2(s32, s32, s32);
extern s32 game_reset(void);
extern s32 indexed_byte_lsb_set(s32, s32);
extern s32 value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern s32 bustup_ptr_init(void);
extern s32 fade_up_main(void);
extern s32 fade_down_main(void);
extern s32 fade_in_step(void);
extern s32 fade_out_step(void);
extern s32 bustup_face_change(s32);
extern s32 zyotai_sprite_set_chk(void);
extern s32 date_name_set(s32);
extern s32 serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern s32 bustup_load_step(s32);
extern s32 memcpy(s32, s32, s32);
extern s32 bunkaken_girl3_cl_bank_init(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern u8 girl3_flag;
extern s16 now_serifu;
extern u8 now_girl;
extern s32 kuchi_stop;
extern s32 bunka_girl2;
extern u8 now_sub_sub_step;

void bunkaken_girl3_cl_bk4_95_exh_main(void) {
    s32 t6 = now_sub_sub_step;
    s32 v0;

    switch ((u32)t6) {
    case 0:
        value_cache_find_or_load(16655, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        v0 = dec_bg_init(1);
        indexed_byte_lsb_set(1, v0);
        sub_sub_step_up();
        break;
    case 3:
        bustup_load_step(22568);
        break;
    case 4:
        memcpy(0x80197000, 0x80180000, 0x16800);
        sub_sub_step_up();
        break;
    case 5:
        bustup_load_step(22748);
        break;
    case 6:
        memcpy(0x801AE000, 0x80180000, 0x16800);
        bustup_ptr_init();
        now_girl = 3;
        zyotai_sprite_set_chk();
        bunkaken_basyo_init3();
        cd_read_start2(9, 0x801E8000, 19210);
        break;
    case 7:
        fade_in_step();
        break;
    case 8:
        v0 = get_g_zyotai_low(3);
        if (v0 == 4 || bunka_girl2 == 3) {
            sub_sub_step_up();
        } else {
            fade_up_main();
        }
        break;
    case 9:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        bunkaken_girl3_cl_bank_init();
        v0 = get_g_zyotai_low(3);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 4;
            break;
        case 2:
            now_serifu = 13;
            break;
        case 3:
            now_serifu = 22;
            break;
        }
        v0 = get_g_zyotai_low(3);
        if (v0 == 4 || bunka_girl2 == 3) {
            now_serifu = 31;
        }
        break;
    case 10:
        serifu_run();
        break;
    case 11:
        v0 = *(s32 *)&girl3_flag;
        if ((v0 << 29) < 0) {
            goto_sub_sub_step(13);
        } else {
            now_serifu = 1;
            girl3_flag = girl3_flag | 4;
            sub_sub_step_up();
        }
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        v0 = get_g_zyotai_low(3);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 5;
            break;
        case 2:
            now_serifu = 14;
            break;
        case 3:
            now_serifu = 23;
            break;
        }
        sub_sub_step_up();
        v0 = get_g_zyotai_low(3);
        if (v0 == 4 || bunka_girl2 == 3) {
            goto_sub_sub_step(36);
        }
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        date_name_set(12);
        kuchi_stop = 1;
        now_girl = 12;
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
        fade_down_main();
        break;
    case 23:
        serifu_run();
        break;
    case 24:
        fade_up_main();
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
        now_girl = 3;
        zyotai_sprite_set_chk();
        date_name_set(3);
        break;
    case 29:
        fade_up_main();
        break;
    case 30:
        serifu_run();
        break;
    case 31:
        date_name_set(12);
        now_girl = 12;
        kuchi_stop = 1;
        sub_sub_step_up();
        break;
    case 32:
        serifu_run();
        break;
    case 33:
        date_name_set(3);
        now_girl = 3;
        kuchi_stop = 0;
        sub_sub_step_up();
        break;
    case 34:
        serifu_run();
        break;
    case 35:
        fade_down_main();
        break;
    case 36:
        fade_out_step();
        break;
    case 37:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 38:
        serifu_run();
        break;
    case 39:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
