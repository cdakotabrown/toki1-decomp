/* @SymbolStart: 0x80139D84 */
/* Address: 0x80139D8C, Size: 2184 bytes (546 insns) */
/* @SymbolSize: 2192 */
/* @RodataBase: 0x8015C7D4 */
/* @PostProc: swap_instructions --offset1 0x250 --offset2 0x254 */
/* @PostProc: swap_instructions --offset1 0x24c --offset2 0x250 */
/* @PostProc: swap_instructions --offset1 0x270 --offset2 0x274 */
/* @PostProc: swap_instructions --offset1 0x4ec --offset2 0x4f0 */
/* @PostProc: swap_instructions --offset1 0x4f4 --offset2 0x4f8 */
/* @PostProc: patch_insn_reg_field 0x23c:rt:t6:t2 0x240:rs:t6:t2 0x240:rt:t6:t2 0x248:rt:t7:t3 0x24c:rt:t6:t2 0x250:rs:t7:t3 0x250:rt:t7:t3 0x254:rt:t9:v1 0x258:rs:t9:v1 0x258:rt:t9:t6 0x260:rt:t7:t3 0x268:rt:t8:t4 0x26c:rs:t9:t6 0x26c:rt:t0:t8 0x270:rs:t0:t8 0x270:rt:t1:t9 0x274:rt:t8:t4 0x278:rs:t1:t9 0x278:rt:t2:t1 0x280:rt:t2:t1 0x294:rt:t3:t5 0x29c:rt:t4:t6 0x2a0:rs:t4:t6 0x2a0:rt:t4:t6 0x2a4:rt:t3:t5 0x2ac:rt:t4:t6 0x2b4:rt:t5:t7 0x2b8:rt:t5:t7 0x2d4:rt:t6:t8 0x2d8:rt:t7:t9 0x2dc:rt:t7:t9 0x2e0:rt:t6:t8 0x30c:rt:t8:t0 0x318:rt:t8:t0 0x398:rt:t9:t1 0x39c:rs:t9:t1 0x3e0:rt:t0:t2 0x3ec:rt:t0:t2 0x424:rt:t1:t3 0x428:rs:t1:t3 0x44c:rt:t3:t5 0x458:rt:t4:t6 0x464:rt:t2:t4 0x470:rt:t2:t4 0x47c:rt:t3:t5 0x484:rt:t4:t6 0x4dc:rt:t5:t2 0x4e0:rs:t5:t2 0x4e0:rt:t5:t2 0x4e8:rt:t6:t3 0x4ec:rt:t5:t2 0x4f0:rs:t6:t3 0x4f0:rt:t6:t3 0x4f4:rt:t8:v1 0x4fc:rs:t8:v1 0x4fc:rt:t8:t7 0x500:rt:t6:t3 0x508:rt:t7:t4 0x50c:rt:t7:t4 0x510:rs:t8:t7 0x514:rt:t0:t1 0x51c:rt:t0:t1 0x530:rt:t1:t5 0x538:rt:t2:t6 0x53c:rs:t2:t6 0x53c:rt:t2:t6 0x540:rt:t1:t5 0x548:rt:t2:t6 0x550:rt:t3:t7 0x554:rt:t3:t7 0x570:rt:t4:t8 0x574:rt:t5:t9 0x578:rt:t5:t9 0x57c:rt:t4:t8 0x5a8:rt:t6:t0 0x5b4:rt:t6:t0 0x634:rt:t7:t1 0x638:rs:t7:t1 0x67c:rt:t8:t2 0x688:rt:t8:t2 0x6e4:rt:t9:t3 0x6e8:rs:t9:t3 0x700:rd:t0:t4 0x708:rt:t0:t4 0x70c:rt:t0:t4 0x714:rs:t0:t4 0x71c:rt:t1:t5 0x728:rt:t1:t5 0x72c:rt:t2:t6 0x738:rt:t2:t6 0x73c:rt:t3:t7 0x748:rt:t3:t7 0x74c:rt:t4:t8 0x754:rt:t4:t8 0x758:rt:t5:t9 0x75c:rs:t5:t9 0x75c:rt:t5:t9 0x760:rt:t6:t0 0x764:rs:t5:t9 0x770:rs:t6:t0 0x770:rt:t6:t0 0x778:rs:t6:t0 0x778:rt:t7:t1 0x77c:rt:t7:t1 0x7b8:rd:t8:t2 0x7c0:rt:t8:t2 0x7c4:rt:t8:t2 0x7cc:rs:t8:t2 0x7d4:rt:t9:t3 0x7e0:rt:t9:t3 0x7e4:rt:t0:t4 0x7f0:rt:t0:t4 0x7f4:rt:t1:t5 0x800:rt:t1:t5 0x868:rt:t2:v1 0x86c:rs:t2:v1 0x86c:rt:t2:t6 0x874:rs:t2:t6 0x874:rt:t3:t8 0x87c:rt:t3:t8 */
#include "types.h"

extern u8 now_sub_sub_step;
extern void sub_sub_step_up();
extern void goto_step(s32);
extern void goto_sub_step(s32);
extern void goto_sub_sub_step(s32);
extern void device_cmd_send(s32);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32  device_load_poll(void);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void set_dec_bri(s32);
extern void dec_bg_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern s32  dec_bg_init(s32);
extern void value_cache_find_or_load(s32, s32);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void zyotai_sprite_set_chk(void);
extern void yazirushi_off(void);
extern void serifu_run(void);
extern void date_x_taku_set(s32, s32);
extern void bustup_load_step(s32);
extern u8   get_g_zyotai_low(s32);
extern void date_name_set(s32);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void bunkasai_girl8_srf_bank_set(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_sub_reset(void);

extern u8  now_girl;
extern s16 now_serifu;
extern u8  ret_step;
extern u8  ret_sub_step;
extern u8  ret_sub_sub_step;
extern u8  now_step;
extern u8  now_sub_step;
extern u8  tt_result;
extern u8  bunkaken_flags;
extern s32 common_disp_off;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkasai_talk_c1(void)
{
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
            s32 t = dec_bg_init(1);
            indexed_byte_lsb_set(1, t);
            sub_sub_step_up();
        }
        break;
    case 3:
        bustup_load_step(20498);
        break;
    case 4:
        bustup_ptr_init();
        now_girl = 8;
        zyotai_sprite_set_chk();
        bunkasai_basyo_init();
        now_serifu = 42;
        break;
    case 5:
        fade_in_step();
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        cd_read_start2(20, 0x801E8000, 18833);
        sub_sub_step_up();
        break;
    case 8:
        fade_up_main();
        break;
    case 9:
        if (get_CD_State2() & 1) {
            switch (get_g_zyotai_low(8)) {
            case 0:
            case 1:
                now_serifu = 61;
                break;
            case 2:
                now_serifu = 62;
                break;
            case 3:
                now_serifu = 63;
                break;
            case 4:
                now_serifu = 64;
                break;
            }
            bunkasai_girl8_srf_bank_set();
        }
        break;
    case 10:
        serifu_run();
        break;
    case 11:
        if (get_g_zyotai_low(8) == 4)
            fade_down_main();
        else
            goto_sub_sub_step(14);
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        goto_sub_sub_step(25);
        break;
    case 14:
        fade_out();
        break;
    case 15:
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = now_sub_sub_step + 1;
        bunkaken_flags = (u8)((((bunkaken_flags | 2) & 0xFFE3) | 0x10));
        goto_step(208);
        break;
    case 16:
        common_disp_off = 1;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(163);
        break;
    case 17:
        device_cmd_send(191);
        device_load_start(1, 0xC486, 0xC459, 0xD266, 0xD216, 0xD20E);
        date_x_taku_set(1, 0);
        bunkasai_basyo_init();
        mes_base_y = 192;
        message_window_init();
        hizuke_init();
        yazirushi_off();
        date_name_set(8);
        common_disp_off = 0;
        sub_sub_step_up();
        break;
    case 18:
        if (device_load_poll() == 1) {
            device_cmd_send(512);
            dec_bg_reset();
            set_dec_bri(0);
            value_cache_find_or_load(16645, 1);
            sub_sub_step_up();
        }
        break;
    case 19:
        if (get_CD_State2() & 1) {
            s32 t = dec_bg_init(1);
            indexed_byte_lsb_set(1, t);
            sub_sub_step_up();
        }
        break;
    case 20:
        bustup_load_step(20498);
        break;
    case 21:
        bustup_ptr_init();
        now_girl = 8;
        zyotai_sprite_set_chk();
        cd_read_start2(20, 0x801E8000, 18833);
        break;
    case 22:
        fade_in();
        break;
    case 23:
        if (get_CD_State2() & 1) {
            switch (get_g_zyotai_low(8)) {
            case 0:
            case 1:
                now_serifu = 66;
                break;
            case 2:
                now_serifu = 67;
                break;
            case 3:
                now_serifu = 68;
                break;
            }
            bunkasai_girl8_srf_bank_set();
        }
        break;
    case 24:
        serifu_run();
        break;
    case 25:
        if (get_g_zyotai_low(8) == 4)
            sub_sub_step_up();
        else
            fade_down_main();
        break;
    case 26:
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = now_sub_sub_step + 1;
        bunkaken_flags = (u8)((bunkaken_flags | 2) & 0xFFE3);
        goto_sub_step(164);
        break;
    case 27:
        common_disp_off = 1;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(163);
        break;
    case 28:
        device_cmd_send(191);
        device_load_start(1, 0xC486, 0xC459, 0xD266, 0xD216, 0xD20E);
        date_x_taku_set(1, 0);
        bunkasai_basyo_init();
        mes_base_y = 192;
        message_window_init();
        hizuke_init();
        yazirushi_off();
        date_name_set(8);
        common_disp_off = 0;
        sub_sub_step_up();
        break;
    case 29:
        if (device_load_poll() == 1) {
            device_cmd_send(512);
            dec_bg_reset();
            set_dec_bri(0);
            value_cache_find_or_load(16645, 1);
            sub_sub_step_up();
        }
        break;
    case 30:
        if (get_CD_State2() & 1) {
            s32 t = dec_bg_init(1);
            indexed_byte_lsb_set(1, t);
            sub_sub_step_up();
        }
        break;
    case 31:
        bustup_load_step(20498);
        break;
    case 32:
        bustup_ptr_init();
        now_girl = 8;
        zyotai_sprite_set_chk();
        cd_read_start2(20, 0x801E8000, 18833);
        break;
    case 33:
        if (get_g_zyotai_low(8) == 4)
            fade_in_step();
        else
            fade_in();
        break;
    case 34:
        if (get_CD_State2() & 1) {
            switch (get_g_zyotai_low(8)) {
            case 0:
            case 1:
                now_serifu = 69;
                break;
            case 2:
                now_serifu = 71;
                break;
            case 3:
                now_serifu = 73;
                break;
            case 4:
                now_serifu = 75;
                break;
            }
            if ((u32)tt_result < 4)
                now_serifu = now_serifu + 1;
            bunkasai_girl8_srf_bank_set();
        }
        break;
    case 35:
        serifu_run();
        break;
    case 36:
        sub_sub_step_up();
        switch (get_g_zyotai_low(8)) {
        case 0:
        case 1:
            now_serifu = 58;
            break;
        case 2:
            now_serifu = 59;
            break;
        case 3:
            now_serifu = 60;
            break;
        case 4:
            goto_sub_sub_step(39);
            break;
        }
        break;
    case 37:
        serifu_run();
        break;
    case 38:
        fade_down_main();
        break;
    case 39:
        fade_out_step();
        break;
    case 40:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 41:
        serifu_run();
        break;
    case 42:
        bunkaken_flags |= 1;
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
