/* @SymbolStart: 0x801350A8 */
/* @RodataBase: 0x8015BE6C */
/* Address: 0x801350B0 */
/* Size: 920 bytes (230 insns) */
/* @SymbolSize: 928 */
/* @PostProc: patch_insn_reg_field 0xc0:rt:t8:v0 0xc4:rs:t8:v0 0xc4:rt:t8:v0 0xcc:rs:t8:v0 0xd0:rt:t0:t9 0xd4:rt:t9:t8 0xe0:rt:t9:t8 0xe8:rt:t0:t9 0x11c:rt:t1:t0 0x124:rt:t2:t1 0x128:rs:t2:t1 0x128:rt:t2:t1 0x12c:rt:t1:t0 0x134:rt:t2:t1 0x13c:rt:t3:t2 0x140:rt:t3:t2 0x16c:rt:t4:t3 0x178:rt:t4:t3 0x17c:rt:t5:t4 0x188:rt:t5:t4 0x1b8:rt:t6:t5 0x1c4:rt:t6:t5 0x1e8:rt:t7:t6 0x1ec:rs:t7:t6 0x288:rt:t9:t8 0x294:rt:t0:t9 0x2a0:rt:t8:t7 0x2ac:rt:t8:t7 0x2b8:rt:t9:t8 0x2c0:rt:t0:t9 0x2e4:rt:t1:t0 0x2e8:rs:t1:t0 0x2e8:rt:t1:t0 0x2f0:rs:t1:t0 0x2f0:rt:t2:t1 0x2f4:rt:t2:t1 0x300:rt:t3:t2 0x304:rs:t3:t2 0x304:rt:t3:t2 0x30c:rs:t3:t2 0x30c:rt:t4:t3 0x310:rt:t4:t3 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkasai_girl2_vst_bank_init(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init2(void);

extern s16 now_serifu;
extern u8  now_year;
extern u8  now_sub_step;
extern u8  now_girl;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

extern u8 now_sub_sub_step;
void bunkasai_main3a(void) {
    s32 v0;

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
        break;
    case 4:
        if (now_year == 95) {
            now_serifu = 46;
        } else {
            now_serifu = 47;
        }
        sub_sub_step_up();
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        fade_out_step();
        break;
    case 7:
        bunka_sd_kind = 2;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 8:
        bustup_load_step(21038);
        break;
    case 9:
        bunkasai_basyo_init2();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 2;
        zyotai_sprite_set_chk();
        cd_read_start2(26, 0x801E8000, 18962);
        break;
    case 10:
        v0 = get_g_zyotai_low(2);
        if (v0 == 4) {
            now_serifu = 195;
            fade_in_step();
        } else {
            sub_sub_step_up();
        }
        break;
    case 11:
        if (get_CD_State2() & 1) {
            bunkasai_girl2_vst_bank_init();
        }
        break;
    case 12:
        v0 = get_g_zyotai_low(2);
        if (v0 == 4) {
            serifu_run();
        } else {
            sub_sub_step_up();
        }
        break;
    case 13:
        v0 = get_g_zyotai_low(2);
        if (v0 == 4) {
            fade_up_main();
        } else {
            fade_in();
        }
        break;
    case 14:
        v0 = get_g_zyotai_low(2);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 189;
            break;
        case 2:
            now_serifu = 191;
            break;
        case 3:
            now_serifu = 193;
            break;
        }
        if ((u32)get_g_zyotai_low(2) < 4) {
            v0 = now_year;
            if (v0 == 96) {
                now_serifu = now_serifu + 9;
            }
            if (v0 == 97) {
                now_serifu = now_serifu + 15;
            }
        }
        sub_sub_step_up();
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        fade_down_main();
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        fade_out_step();
        break;
    case 19:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 20:
        serifu_run();
        break;
    case 21:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
