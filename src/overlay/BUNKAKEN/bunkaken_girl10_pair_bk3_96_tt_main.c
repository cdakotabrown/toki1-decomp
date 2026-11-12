/* @SymbolStart: 0x8013DBE8 */
/* @RodataBase: 0x80154130 */
/* @SymbolSize: 1752 */
/* @PostProc: swap_instructions --offset1 0x184 --offset2 0x188 */
/* @PostProc: swap_instructions --offset1 0x180 --offset2 0x184 */
/* @PostProc: swap_instructions --offset1 0x1a4 --offset2 0x1a8 */
/* @PostProc: swap_instructions --offset1 0x3fc --offset2 0x400 */
/* @PostProc: swap_instructions --offset1 0x404 --offset2 0x408 */
/* @PostProc: patch_insn_reg_field 0x170:rt:t2:t8 0x174:rs:t2:t8 0x174:rt:t2:t8 0x17c:rt:t3:t9 0x180:rt:t2:t8 0x184:rs:t3:t9 0x184:rt:t3:t9 0x188:rt:t5:v1 0x18c:rs:t5:v1 0x18c:rt:t5:t2 0x194:rt:t3:t9 0x19c:rt:t4:t0 0x1a0:rs:t5:t2 0x1a0:rt:t6:t4 0x1a4:rs:t6:t4 0x1a4:rt:t7:t5 0x1a8:rt:t4:t0 0x1ac:rs:t7:t5 0x1ac:rt:t8:t7 0x1b4:rt:t8:t7 0x1c8:rt:t9:t1 0x1d0:rt:t0:t2 0x1d4:rs:t0:t2 0x1d4:rt:t0:t2 0x1d8:rt:t9:t1 0x1e0:rt:t0:t2 0x1e8:rt:t1:t3 0x1ec:rt:t1:t3 0x208:rt:t2:t4 0x20c:rt:t3:t5 0x210:rt:t3:t5 0x214:rt:t2:t4 0x240:rt:t4:t6 0x24c:rt:t4:t6 0x2cc:rt:t5:t7 0x2d0:rs:t5:t7 0x314:rt:t6:t8 0x320:rt:t6:t8 0x358:rt:t7:t9 0x35c:rs:t7:t9 0x380:rt:t9:t1 0x38c:rt:t0:t2 0x398:rt:t8:t0 0x3a4:rt:t8:t0 0x3b0:rt:t9:t1 0x3b8:rt:t0:t2 0x3ec:rt:t1:t8 0x3f0:rs:t1:t8 0x3f0:rt:t1:t8 0x3f8:rt:t2:t9 0x3fc:rt:t1:t8 0x400:rs:t2:t9 0x400:rt:t2:t9 0x404:rt:t4:v1 0x40c:rs:t4:v1 0x40c:rt:t4:t3 0x410:rt:t2:t9 0x418:rt:t3:t0 0x41c:rt:t3:t0 0x420:rs:t4:t3 0x424:rt:t6:t7 0x42c:rt:t6:t7 0x440:rt:t7:t1 0x448:rt:t8:t2 0x44c:rs:t8:t2 0x44c:rt:t8:t2 0x450:rt:t7:t1 0x458:rt:t8:t2 0x460:rt:t9:t3 0x464:rt:t9:t3 0x480:rt:t0:t4 0x484:rt:t1:t5 0x488:rt:t1:t5 0x48c:rt:t0:t4 0x4b8:rt:t2:t6 0x4c4:rt:t2:t6 0x544:rt:t3:t7 0x548:rs:t3:t7 0x58c:rt:t4:t8 0x598:rt:t4:t8 0x5d0:rt:t5:t9 0x5d4:rs:t5:t9 0x5e8:rt:t9:t3 0x5fc:rt:t7:t1 0x608:rt:t8:t2 0x614:rt:t6:t0 0x620:rt:t6:t0 0x62c:rt:t7:t1 0x634:rt:t8:t2 0x638:rs:t9:t3 0x638:rt:t9:t3 0x63c:rt:t0:t4 0x640:rs:t9:t3 0x64c:rs:t0:t4 0x64c:rt:t0:t4 0x654:rs:t0:t4 0x654:rt:t1:t5 0x658:rt:t1:t5 0x6b0:rt:t2:v1 0x6b4:rs:t2:v1 0x6b4:rt:t2:t6 0x6bc:rs:t2:t6 0x6bc:rt:t3:t8 0x6c4:rt:t3:t8 */
/* Address: 0x8013DBF0 */
/* Size: 1744 bytes (436 insns) */
#include "types.h"

extern u8 now_sub_sub_step;

extern s32 sub_sub_step_up(void);
extern s32 goto_step(s32);
extern s32 goto_sub_step(s32);
extern s32 device_cmd_send(s32);
extern s32 device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);
extern s32 get_CD_State2(void);
extern s32 cd_read_start2(s32, s32, s32);
extern s32 game_reset(void);
extern s32 set_dec_bri(s32);
extern s32 dec_bg_reset(void);
extern s32 indexed_byte_lsb_set(s32, s32);
extern s32 value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern s32 hizuke_init(void);
extern s32 message_window_init(void);
extern s32 bustup_ptr_init(void);
extern s32 fade_down_main(void);
extern s32 fade_in(void);
extern s32 fade_out(void);
extern s32 zyotai_sprite_set_chk(void);
extern s32 yazirushi_off(void);
extern s32 serifu_run(void);
extern s32 date_x_taku_set(s32, s32);
extern s32 get_g_zyotai_low(s32);
extern s32 bustup_load_step(s32);
extern s32 date_name_set(s32);
extern s32 bunkaken_girl10_pair_bank_init(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern s16 now_serifu;
extern u8 ret_step;
extern u8 ret_sub_step;
extern u8 ret_sub_sub_step;
extern u8 now_girl;
extern u8 now_step;
extern u8 now_sub_step;
extern u8 tt_result;
extern u8 bunkaken_flags;
extern s32 common_disp_off;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkaken_girl10_pair_bk3_96_tt_main(void)
{
    s32 v0;

    switch ((u32)now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16645, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out();
        break;
    case 2:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        bunkaken_basyo_init3();
        v0 = dec_bg_init(1);
        indexed_byte_lsb_set(1, v0);
        cd_read_start2(13, 0x801E8000, 19314);
        sub_sub_step_up();
        break;
    case 3:
        fade_in();
        break;
    case 4:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        v0 = get_g_zyotai_low(10);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 35;
            break;
        case 2:
            now_serifu = 36;
            break;
        case 3:
            now_serifu = 37;
            break;
        }
        bunkaken_girl10_pair_bank_init();
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        fade_out();
        break;
    case 7:
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = now_sub_sub_step + 1;
        bunkaken_flags = ((bunkaken_flags | 2) & 0xFFE3) | 0xC;
        goto_step(208);
        break;
    case 8:
        common_disp_off = 1;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(140);
        break;
    case 9:
        device_cmd_send(191);
        device_load_start(1, 0xC486, 0xC459, 0xD266, 0xD216, 0xD20E);
        date_x_taku_set(1, 0);
        bunkaken_basyo_init3();
        mes_base_y = 192;
        message_window_init();
        hizuke_init();
        yazirushi_off();
        date_name_set(10);
        common_disp_off = 0;
        sub_sub_step_up();
        break;
    case 10:
        v0 = device_load_poll();
        if (v0 != 1) {
            break;
        }
        device_cmd_send(512);
        dec_bg_reset();
        set_dec_bri(0);
        value_cache_find_or_load(16645, 1);
        sub_sub_step_up();
        break;
    case 11:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        v0 = dec_bg_init(1);
        indexed_byte_lsb_set(1, v0);
        sub_sub_step_up();
        break;
    case 12:
        bustup_load_step(26933);
        break;
    case 13:
        bustup_ptr_init();
        now_girl = 10;
        zyotai_sprite_set_chk();
        cd_read_start2(13, 0x801E8000, 19314);
        break;
    case 14:
        fade_in();
        break;
    case 15:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        v0 = get_g_zyotai_low(10);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 38;
            break;
        case 2:
            now_serifu = 39;
            break;
        case 3:
            now_serifu = 40;
            break;
        }
        bunkaken_girl10_pair_bank_init();
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        fade_down_main();
        break;
    case 18:
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = now_sub_sub_step + 1;
        bunkaken_flags = (bunkaken_flags | 2) & 0xFFE3;
        goto_sub_step(141);
        break;
    case 19:
        common_disp_off = 1;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(140);
        break;
    case 20:
        device_cmd_send(191);
        device_load_start(1, 0xC486, 0xC459, 0xD266, 0xD216, 0xD20E);
        date_x_taku_set(1, 0);
        bunkaken_basyo_init3();
        mes_base_y = 192;
        message_window_init();
        hizuke_init();
        yazirushi_off();
        date_name_set(10);
        common_disp_off = 0;
        sub_sub_step_up();
        break;
    case 21:
        v0 = device_load_poll();
        if (v0 != 1) {
            break;
        }
        device_cmd_send(512);
        dec_bg_reset();
        set_dec_bri(0);
        value_cache_find_or_load(16645, 1);
        sub_sub_step_up();
        break;
    case 22:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        v0 = dec_bg_init(1);
        indexed_byte_lsb_set(1, v0);
        sub_sub_step_up();
        break;
    case 23:
        bustup_load_step(26933);
        break;
    case 24:
        bustup_ptr_init();
        now_girl = 10;
        zyotai_sprite_set_chk();
        cd_read_start2(13, 0x801E8000, 19314);
        break;
    case 25:
        fade_in();
        break;
    case 26:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        v0 = get_g_zyotai_low(10);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 41;
            break;
        case 2:
            now_serifu = 43;
            break;
        case 3:
            now_serifu = 45;
            break;
        }
        if ((u32)tt_result < 3) {
            now_serifu = now_serifu + 1;
        }
        bunkaken_girl10_pair_bank_init();
        break;
    case 27:
        serifu_run();
        break;
    case 28:
        fade_out();
        break;
    case 29:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 30:
        serifu_run();
        break;
    case 31:
        bunkaken_flags = bunkaken_flags | 1;
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
