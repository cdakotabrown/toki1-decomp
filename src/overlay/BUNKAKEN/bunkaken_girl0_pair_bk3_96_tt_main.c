/* @SymbolStart: 0x801339F4 */
/* @RodataBase: 0x80153250 */
/* @SymbolSize: 1232 */
/* @PostProc: swap_instructions --offset1 0x180 --offset2 0x184 */
/* @PostProc: swap_instructions --offset1 0x188 --offset2 0x18c */
/* @PostProc: patch_insn_reg_field 0x170:rt:t2:t7 0x174:rs:t2:t7 0x174:rt:t2:t7 0x17c:rt:t3:t8 0x180:rt:t2:t7 0x184:rs:t3:t8 0x184:rt:t3:t8 0x188:rt:t5:v1 0x190:rs:t5:v1 0x190:rt:t5:t2 0x194:rt:t3:t8 0x19c:rt:t4:t9 0x1a0:rt:t4:t9 0x1a4:rs:t5:t2 0x1a4:rt:t6:t4 0x1a8:rs:t6:t4 0x1a8:rt:t7:t6 0x1b0:rt:t7:t6 0x1c4:rt:t8:t0 0x1cc:rt:t9:t1 0x1d0:rs:t9:t1 0x1d0:rt:t9:t1 0x1d4:rt:t8:t0 0x1dc:rt:t9:t1 0x1e4:rt:t0:t2 0x1e8:rt:t0:t2 0x204:rt:t1:t3 0x208:rt:t2:t4 0x20c:rt:t2:t4 0x210:rt:t1:t3 0x23c:rt:t3:t5 0x248:rt:t3:t5 0x2c8:rt:t4:t6 0x2cc:rs:t4:t6 0x350:rt:t5:t7 0x354:rs:t5:t7 0x368:rt:t9:t1 0x37c:rt:t7:t9 0x388:rt:t8:t0 0x394:rt:t6:t8 0x3a0:rt:t6:t8 0x3ac:rt:t7:t9 0x3b4:rt:t8:t0 0x3b8:rs:t9:t1 0x3b8:rt:t9:t1 0x3bc:rt:t0:t2 0x3c0:rs:t9:t1 0x3cc:rs:t0:t2 0x3cc:rt:t0:t2 0x3d4:rs:t0:t2 0x3d4:rt:t1:t3 0x3d8:rt:t1:t3 0x418:rt:t3:t5 0x424:rt:t4:t6 0x430:rt:t2:t4 0x43c:rt:t2:t4 0x448:rt:t3:t5 0x450:rt:t4:t6 0x4a8:rt:t5:v1 0x4ac:rs:t5:v1 0x4ac:rt:t5:t7 0x4b4:rs:t5:t7 0x4b4:rt:t6:t9 0x4bc:rt:t6:t9 */
/* Address: 0x801339FC */
/* Size: 1224 bytes (306 insns) */
#include "types.h"

extern u8 now_sub_sub_step;

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
extern s32 yazirushi_off(void);
extern s32 serifu_run(void);
extern s32 date_x_taku_set(s32, s32);
extern s32 get_g_zyotai_low(s32);
extern s32 bustup_load_step(s32);
extern s32 date_name_set(s32);
extern s32 bunkaken_girl0_pair_bank_init(void);
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

void bunkaken_girl0_pair_bk3_96_tt_main(void) {
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
        cd_read_start2(15, 0x801E8000, 18947);
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
        v0 = get_g_zyotai_low(0);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 74;
            break;
        case 2:
            now_serifu = 78;
            break;
        case 3:
            now_serifu = 82;
            break;
        }
        bunkaken_girl0_pair_bank_init();
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
        date_name_set(0);
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
        bustup_load_step(21578);
        break;
    case 13:
        bustup_ptr_init();
        now_girl = 0;
        zyotai_sprite_set_chk();
        cd_read_start2(15, 0x801E8000, 18947);
        break;
    case 14:
        fade_in();
        break;
    case 15:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        v0 = get_g_zyotai_low(0);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 75;
            break;
        case 2:
            now_serifu = 79;
            break;
        case 3:
            now_serifu = 83;
            break;
        }
        if ((u32)tt_result < 2) {
            now_serifu = now_serifu + 1;
        }
        bunkaken_girl0_pair_bank_init();
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        v0 = get_g_zyotai_low(0);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 77;
            break;
        case 2:
            now_serifu = 81;
            break;
        case 3:
            now_serifu = 85;
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
