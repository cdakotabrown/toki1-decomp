/* @SymbolStart: 0x80138A08 */
/* @SymbolSize: 1200 */
/* @RodataBase: 0x8015395C */
/* @PostProc: swap_instructions --offset1 0x174 --offset2 0x178 */
/* @PostProc: patch_insn_reg_field 0x164:rt:t1:t6 0x168:rs:t1:t6 0x168:rt:t1:t6 0x170:rt:t2:t7 0x174:rt:t1:t6 0x178:rs:t2:t7 0x178:rt:t2:t7 0x17c:rt:at:v1 0x180:rt:t4:at 0x184:rs:t4:v1 0x184:rt:t4:t1 0x188:rt:t2:t7 0x190:rt:t3:t8 0x194:rt:t3:t8 0x198:rs:t4:t1 0x198:rt:t5:t3 0x19c:rs:t5:t3 0x19c:rt:t6:t5 0x1a4:rt:t6:t5 0x1b8:rt:t7:t9 0x1c0:rt:t8:t0 0x1c4:rs:t8:t0 0x1c4:rt:t8:t0 0x1c8:rt:t7:t9 0x1d0:rt:t8:t0 0x1d8:rt:t9:t1 0x1dc:rt:t9:t1 0x1f8:rt:t0:t2 0x1fc:rt:t1:t3 0x200:rt:t1:t3 0x204:rt:t0:t2 0x230:rt:t2:t4 0x23c:rt:t2:t4 0x2bc:rt:t3:t5 0x2c0:rs:t3:t5 0x304:rt:t4:t6 0x310:rt:t4:t6 0x348:rt:t5:t7 0x34c:rs:t5:t7 0x360:rt:t8:t0 0x374:rt:t7:t9 0x38c:rt:t6:t8 0x398:rt:t6:t8 0x3a0:rt:t7:t9 0x3a4:rs:t8:t0 0x3a4:rt:t8:t0 0x3a8:rt:t9:t1 0x3ac:rs:t8:t0 0x3b8:rs:t9:t1 0x3b8:rt:t9:t1 0x3c0:rs:t9:t1 0x3c0:rt:t0:t2 0x3c4:rt:t0:t2 0x404:rt:t2:t4 0x41c:rt:t1:t3 0x428:rt:t1:t3 0x430:rt:t2:t4 0x488:rt:t3:v1 0x48c:rs:t3:v1 0x48c:rt:t3:t5 0x494:rs:t3:t5 0x494:rt:t4:t7 0x49c:rt:t4:t7 */
/* Address: 0x80138A10 (window starts at the absorbed cross-prologue head */
/* Size: 1200 bytes (300 insns incl. head) */
#include "types.h"

extern s32 sub_sub_step_up(void);
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
extern s32 date_name_set(s32);
extern s32 yazirushi_off(void);
extern s32 serifu_run(void);
extern s32 date_x_taku_set(s32, s32);
extern s32 get_g_zyotai_low(s32);
extern s32 bustup_load_step(s32);
extern void bunkaken_girl4_pair_bank_init(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern s16 now_serifu;
extern u8  ret_step;
extern u8  ret_sub_step;
extern u8  ret_sub_sub_step;
extern u8  now_girl;
extern u8  now_step;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern u8  tt_result;
extern u8  bunkaken_flags;
extern s32 common_disp_off;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkaken_girl4_pair_bk3_96_tt_main(void) {
    s32 v0;

    switch ((s32)now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16645, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkaken_basyo_init3();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            cd_read_start2(12, 0x801E8000, 19282);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(4);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 52;
                break;
            case 2:
            case 3:
                now_serifu = 53;
                break;
            }
            bunkaken_girl4_pair_bank_init();
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        fade_down_main();
        break;
    case 7:
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = now_sub_sub_step + 1;
        bunkaken_flags = (bunkaken_flags | 2) & 0xFFE3;
        goto_sub_step(141);
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
        date_name_set(4);
        common_disp_off = 0;
        sub_sub_step_up();
        break;
    case 10:
        if (device_load_poll() == 1) {
            device_cmd_send(512);
            dec_bg_reset();
            set_dec_bri(0);
            value_cache_find_or_load(16645, 1);
            sub_sub_step_up();
        }
        break;
    case 11:
        if (get_CD_State2() & 1) {
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 12:
        bustup_load_step(26303);
        break;
    case 13:
        bustup_ptr_init();
        now_girl = 4;
        zyotai_sprite_set_chk();
        cd_read_start2(12, 0x801E8000, 19282);
        break;
    case 14:
        fade_in();
        break;
    case 15:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(4);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 54;
                break;
            case 2:
            case 3:
                now_serifu = 56;
                break;
            }
            if ((u32)tt_result < 2) {
                now_serifu = now_serifu + 1;
            }
            bunkaken_girl4_pair_bank_init();
        }
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        v0 = get_g_zyotai_low(4);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 58;
            break;
        case 2:
        case 3:
            now_serifu = 59;
            break;
        }
        sub_sub_step_up();
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        fade_out();
        break;
    case 20:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 21:
        serifu_run();
        break;
    case 22:
        bunkaken_flags = bunkaken_flags | 1;
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
