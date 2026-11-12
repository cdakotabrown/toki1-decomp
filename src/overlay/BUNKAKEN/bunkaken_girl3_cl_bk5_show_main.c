/* @SymbolStart: 0x80143908 */
/* @SymbolSize: 1144 */
/* @RodataBase: 0x801548FC */
/* Address: 0x80143910 */
/* Size: 1136 bytes (284 insns) + 8-byte cross-prologue head at 0x80143908 */
/* @PostProc: patch_insn_reg_field 0x1b4:rt:t6:t7 0x1b8:rd:t5:t6 0x1bc:rs:t5:t6 0x1d4:rs:t6:t7 0x1d4:rt:t6:t8 0x1d8:rt:t8:t0 0x1dc:rs:t6:t8 0x1dc:rt:t7:t9 0x1e0:rt:t7:t9 0x1ec:rt:t8:t0 0x200:rt:t9:t1 0x20c:rt:t9:t1 0x230:rt:t0:t2 0x23c:rt:t0:t2 0x25c:rt:t1:t3 0x260:rs:t1:t3 0x2a4:rt:t2:t4 0x2a8:rs:t2:t4 0x2a8:rt:t2:t4 0x2b0:rs:t2:t4 0x2f4:rt:t6:t8 0x300:rt:t9:t1 0x30c:rt:t3:t5 0x310:rs:t3:t5 0x310:rt:t3:t5 0x318:rs:t3:t5 0x320:rd:t4:t6 0x324:rs:t4:t6 0x324:rt:t5:t7 0x32c:rt:t5:t7 0x330:rs:t6:t8 0x330:rt:t6:t8 0x338:rs:t6:t8 0x340:rd:t7:t9 0x344:rs:t7:t9 0x344:rt:t8:t0 0x34c:rt:t8:t0 0x350:rs:t9:t1 0x350:rt:t9:t1 0x358:rs:t9:t1 0x35c:rd:t0:t2 0x360:rs:t0:t2 0x360:rt:t1:t3 0x364:rt:t1:t3 0x378:rt:t2:t4 0x37c:rs:t2:t4 0x37c:rt:t2:t4 0x384:rs:t2:t4 0x388:rt:t3:t5 0x390:rt:t3:t5 0x3c4:rt:t4:t6 0x3c8:rs:t4:t6 0x3c8:rt:t4:t6 0x3d0:rs:t4:t6 0x408:rt:t5:t7 0x40c:rs:t5:t7 0x40c:rt:t5:t7 0x414:rs:t5:t7 */
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
extern void bustup_ptr_init(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32  get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkaken_girl3_cl_bank_init(void);
extern void bunkaken_sub_reset(void);
extern void bunkaken_basyo_init(void);
extern void bunkaken_basyo_init2(void);
extern void bunkaken_basyo_init3(void);

extern s32 bunka_girl2;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 now_serifu;
extern u8  now_year;
extern s32 girl3_flag;
extern u8  girl3_flag_b;
extern u8  now_girl;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;

void bunkaken_girl3_cl_bk5_show_main(void)
{
    s32 idx = now_sub_sub_step;
    s32 v0;
    s32 ret;

    switch (idx) {
    case 0:
        value_cache_find_or_load(18044, 1);
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
            cd_read_start2(9, 0x801E8000, 19210);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            now_serifu = 63;
            bunkaken_girl3_cl_bank_init();
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        fade_out_step();
        break;
    case 7:
        bunka_sd_kind = 4;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 8:
        cd_read_start2(9, 0x801E8000, 19210);
        sub_sub_step_up();
        break;
    case 9:
        if (get_CD_State2() & 1) {
            bustup_load_step(22703);
        }
        break;
    case 10:
        bunkaken_basyo_init2();
        bunkaken_girl3_cl_bank_init();
        {
            s32 t4 = girl3_flag;
            s32 t0 = 2;
            s32 t6 = t4 << 29;
            if (t6 < 0) {
                goto_sub_sub_step(12);
            } else {
                u8 t8 = girl3_flag_b;
                girl3_flag_b = t8 | 4;
                now_serifu = (s16)t0;
            }
        }
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        now_serifu = 3;
        sub_sub_step_up();
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        bustup_ptr_init();
        now_girl = 3;
        zyotai_sprite_set_chk();
        value_cache_find_or_load(16628, 1);
        break;
    case 15:
        if (get_CD_State2() & 1) {
            bunkaken_basyo_init();
            ret = dec_bg_init(1);
            indexed_byte_lsb_set(1, ret);
            sub_sub_step_up();
        }
        break;
    case 16:
        v0 = get_g_zyotai_low(3);
        if (v0 == 4 || bunka_girl2 == 3U) {
            fade_in_step();
        } else {
            fade_in();
        }
        break;
    case 17:
        v0 = get_g_zyotai_low(3);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = (s16)(now_year * 3 - 221);
            break;
        case 2:
            now_serifu = (s16)(now_year * 3 - 220);
            break;
        case 3:
            now_serifu = (s16)(now_year * 3 - 219);
            break;
        }
        v0 = get_g_zyotai_low(3);
        if (v0 == 4 || bunka_girl2 == 3U) {
            now_serifu = 73;
        }
        sub_sub_step_up();
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        v0 = get_g_zyotai_low(3);
        if (v0 == 4 || bunka_girl2 == 3U) {
            fade_out_step();
        } else {
            fade_down_main();
        }
        break;
    case 20:
        v0 = get_g_zyotai_low(3);
        if (v0 == 4 || bunka_girl2 == 3U) {
            sub_sub_step_up();
        } else {
            fade_out_step();
        }
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
