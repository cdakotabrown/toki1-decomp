/* @SymbolStart: 0x8013B25C */
/* Address: 0x8013B264, Size: 1060 bytes (265 insns) */
/* @SymbolSize: 1068 */
/* @RodataBase: 0x8015CA40 */
/* @PostProc: patch_insn_reg_field 0x170:rt:t1:v0 0x174:rs:t1:v0 0x174:rt:t1:v0 0x180:rd:t2:t1 0x180:rt:t1:v0 0x184:rt:t2:t1 0x188:rt:t4:t3 0x194:rt:t4:t3 0x1b0:rt:t5:v0 0x1b4:rs:t5:v0 0x1b4:rt:t5:v0 0x1bc:rs:t5:v0 0x1fc:rt:t6:v0 0x200:rs:t6:v0 0x200:rt:t6:v0 0x208:rs:t6:v0 0x254:rt:t7:t4 0x25c:rt:t8:t5 0x260:rs:t8:t5 0x260:rt:t8:t5 0x264:rt:t7:t4 0x26c:rt:t8:t5 0x274:rt:t9:t6 0x278:rt:t9:t6 0x2a4:rt:t0:t7 0x2b0:rt:t0:t7 0x2b4:rt:t1:t8 0x2c0:rt:t1:t8 0x2f8:rt:t2:t9 0x2fc:rs:t2:t9 0x314:rd:t3:t0 0x31c:rt:t3:t0 0x320:rt:t3:t0 0x328:rs:t3:t0 0x37c:rd:t4:t1 0x380:rt:t4:t1 0x388:rd:t5:t2 0x38c:rd:t6:t3 0x38c:rt:t5:t2 0x394:rt:t6:t3 0x3a0:rs:t6:t3 0x3a0:rt:t8:t5 0x3a8:rt:t8:t5 */
#include "types.h"

extern void sub_sub_step_up();
extern void goto_step(s32);
extern void goto_sub_sub_step(s32);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern s32  dec_bg_init(s32);
extern void value_cache_find_or_load(s32, s32);
extern void bustup_ptr_init(void);
extern void fade_down_main(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern void bustup_load_step(s32);
extern u8   get_g_zyotai_low(s32);
extern void bunkasai_basyo_init();
extern void bunkasai_girl2_home_srf_bank_set(void);
extern void bunkasai_sub_reset(void);

extern u8  now_girl;
extern s16 now_serifu;
extern u8  now_sub_step;
extern u8  now_year;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

extern u8 now_sub_sub_step;
void bunkasai_step3c(void) {
    switch (now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16636, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1)
            bustup_load_step(21083);
        break;
    case 3:
        {
            s32 t = dec_bg_init(1);
            indexed_byte_lsb_set(1, t);
            bunkasai_basyo_init();
        }
        bustup_ptr_init();
        now_girl = 2;
        zyotai_sprite_set_chk();
        cd_read_start2(10, 0x801E8000, 18988);
        break;
    case 4:
        fade_in();
        break;
    case 5:
        if (get_CD_State2() & 1) {
            switch (get_g_zyotai_low(2)) {
            case 0:
            case 1:
                now_serifu = 2;
                break;
            case 2:
                now_serifu = 3;
                break;
            case 3:
                now_serifu = 4;
                break;
            case 4:
                now_serifu = 5;
                break;
            }
            now_serifu = (s16)(now_serifu + now_year * 4 - 380);
            bunkasai_girl2_home_srf_bank_set();
        }
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        if (now_year == 95 && get_g_zyotai_low(2) == 4) {
            now_serifu = 69;
            fade_down_main();
        } else {
            fade_out();
        }
        break;
    case 8:
        if (now_year == 95 && get_g_zyotai_low(2) == 4)
            serifu_run();
        else
            goto_sub_sub_step(10);
        break;
    case 9:
        fade_out_step();
        break;
    case 10:
        bunka_sd_kind = 3;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 11:
        bustup_load_step(21083);
        break;
    case 12:
        bunkasai_basyo_init();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 2;
        zyotai_sprite_set_chk();
        cd_read_start2(10, 0x801E8000, 18988);
        break;
    case 13:
        fade_in();
        break;
    case 14:
        if (get_CD_State2() & 1) {
            switch (get_g_zyotai_low(2)) {
            case 0:
            case 1:
                now_serifu = 14;
                break;
            case 2:
                now_serifu = 15;
                break;
            case 3:
                now_serifu = 16;
                break;
            case 4:
                now_serifu = 17;
                break;
            }
            now_serifu = (s16)(now_serifu + now_year * 4 - 380);
            if (now_year == 95)
                now_serifu = now_serifu + 12;
            bunkasai_girl2_home_srf_bank_set();
        }
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        fade_down_main();
        break;
    case 17:
        fade_out_step();
        break;
    case 18:
        now_serifu = 1;
        sub_sub_step_up();
        break;
    case 19:
        serifu_run();
        break;
    case 20:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
