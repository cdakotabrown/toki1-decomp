/* @SymbolStart: 0x8013DE3C */
/* @SymbolSize: 1888 */
/* @RodataBase: 0x8015CF0C */
/* @PostProc: swap_instructions --offset1 0x2e4 --offset2 0x2e8 */
/* @PostProc: patch_insn_reg_field 0x2d4:rt:t2:t7 0x2d8:rs:t2:t7 0x2d8:rt:t2:t7 0x2e0:rt:t3:t8 0x2e4:rt:t2:t7 0x2e8:rs:t3:t8 0x2e8:rt:t3:t8 0x2ec:rt:at:v1 0x2f0:rt:t5:at 0x2f4:rs:t5:v1 0x2f4:rt:t5:t2 0x2f8:rt:t3:t8 0x300:rt:t4:t9 0x304:rt:t4:t9 0x308:rs:t5:t2 0x308:rt:t6:t4 0x30c:rs:t6:t4 0x30c:rt:t7:t6 0x314:rt:t7:t6 0x328:rt:t8:t0 0x330:rt:t9:t1 0x334:rs:t9:t1 0x334:rt:t9:t1 0x338:rt:t8:t0 0x340:rt:t9:t1 0x348:rt:t0:t2 0x34c:rt:t0:t2 0x368:rt:t1:t3 0x36c:rt:t2:t4 0x370:rt:t2:t4 0x374:rt:t1:t3 0x3a0:rt:t3:t5 0x3ac:rt:t3:t5 0x42c:rt:t4:t6 0x430:rs:t4:t6 0x474:rt:t5:t7 0x480:rt:t5:t7 0x4dc:rt:t6:t8 0x4e0:rs:t6:t8 0x4f8:rd:t7:t9 0x500:rt:t7:t9 0x504:rt:t7:t9 0x50c:rs:t7:t9 0x550:rt:t8:t0 0x554:rs:t8:t0 0x554:rt:t8:t0 0x560:rs:t8:t0 0x568:rt:t0:t2 0x570:rt:t0:t2 0x59c:rt:t1:t3 0x5a0:rs:t1:t3 0x5a0:rt:t1:t3 0x5a8:rs:t1:t3 0x5a8:rt:t2:t4 0x5ac:rs:t2:t4 0x5c4:rd:t3:t5 0x5cc:rt:t3:t5 0x5d0:rt:t3:t5 0x5d8:rs:t3:t5 0x630:rd:t4:t6 0x638:rt:t4:t6 0x63c:rt:t4:t6 0x644:rs:t4:t6 0x684:rt:t5:t7 0x688:rs:t5:t7 0x688:rt:t5:t7 0x694:rd:t6:t9 0x694:rt:t5:t7 0x698:rs:t6:t9 0x6a4:rd:t7:t0 0x6a8:rt:t7:t0 0x6ac:rt:t9:t2 0x6b0:rs:t9:t2 0x6b0:rt:t9:t2 0x6bc:rd:t0:t3 0x6bc:rt:t9:t2 0x6c0:rs:t0:t3 0x6c4:rt:t2:t5 0x6d0:rt:t2:t5 0x738:rt:t3:v1 0x73c:rs:t3:v1 0x73c:rt:t3:t6 0x744:rs:t3:t6 0x744:rt:t4:t8 0x74c:rt:t4:t8 */
/* Address: 0x8013DE3C (absorbed head) / 0x8013DE44 (prologue) */
/* Size: 1888 bytes (472 insns) incl. the absorbed 2-word head */
#include "types.h"

extern void sub_sub_step_up(void);
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
extern void value_cache_find_or_load(s32, s32);
extern s32  dec_bg_init(s32);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void date_name_set(s32);
extern void yazirushi_off(void);
extern void serifu_run(void);
extern void date_x_taku_set(s32, s32);
extern s32  get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkasai_girl3_clubs2_3_srf_bank_set(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

extern s16 now_serifu;
extern u8  ret_step;
extern u8  ret_sub_step;
extern u8  ret_sub_sub_step;
extern s16 girl3_syoushin;
extern u8  girl3_flag;
extern u8  girl3_basyo;
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

void bunkasai_girl3_club3_96_scene(void) {
    s32 idx;
    s32 v0;
    s32 sel;

    idx = now_sub_sub_step;
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
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 3:
        bustup_load_step(22703);
        break;
    case 4:
        bustup_ptr_init();
        now_girl = 3;
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
        cd_read_start2(23, 0x801E8000, 19158);
        sub_sub_step_up();
        break;
    case 8:
        fade_up_main();
        break;
    case 9:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(3);
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
            case 4:
                now_serifu = 144;
                break;
            }
            bunkasai_girl3_clubs2_3_srf_bank_set();
        }
        break;
    case 10:
        serifu_run();
        break;
    case 11:
        sub_sub_step_up();
        v0 = get_g_zyotai_low(3);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 141;
            break;
        case 2:
            now_serifu = 142;
            break;
        case 3:
            now_serifu = 143;
            break;
        case 4:
            goto_sub_sub_step(13);
            break;
        }
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        if (get_g_zyotai_low(3) == 4) {
            fade_down_main();
        } else {
            goto_sub_sub_step(15);
        }
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        if (get_g_zyotai_low(3) == 4) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 16:
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = now_sub_sub_step + 1;
        bunkaken_flags = (bunkaken_flags | 2) & 0xFFE3;
        goto_sub_step(164);
        break;
    case 17:
        common_disp_off = 1;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(163);
        break;
    case 18:
        device_cmd_send(191);
        device_load_start(1, 0xC486, 0xC459, 0xD266, 0xD216, 0xD20E);
        date_x_taku_set(1, 0);
        bunkasai_basyo_init();
        mes_base_y = 192;
        message_window_init();
        hizuke_init();
        yazirushi_off();
        date_name_set(3);
        common_disp_off = 0;
        sub_sub_step_up();
        break;
    case 19:
        if (device_load_poll() == 1) {
            device_cmd_send(512);
            dec_bg_reset();
            set_dec_bri(0);
            value_cache_find_or_load(16645, 1);
            sub_sub_step_up();
        }
        break;
    case 20:
        if (get_CD_State2() & 1) {
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 21:
        bustup_load_step(22703);
        break;
    case 22:
        bustup_ptr_init();
        now_girl = 3;
        zyotai_sprite_set_chk();
        cd_read_start2(23, 0x801E8000, 19158);
        break;
    case 23:
        if (get_g_zyotai_low(3) == 4) {
            fade_in_step();
        } else {
            fade_in();
        }
        break;
    case 24:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(3);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 146;
                break;
            case 2:
                now_serifu = 148;
                break;
            case 3:
                now_serifu = 150;
                break;
            case 4:
                now_serifu = 152;
                break;
            }
            if (tt_result < 2U) {
                now_serifu = now_serifu + 1;
            }
            bunkasai_girl3_clubs2_3_srf_bank_set();
        }
        break;
    case 25:
        serifu_run();
        break;
    case 26:
        sub_sub_step_up();
        if ((girl3_syoushin & 3) == 0) {
            sel = get_g_zyotai_low(3);
            switch (sel) {
            case 0:
            case 1:
                now_serifu = 23;
                break;
            case 2:
                now_serifu = 25;
                break;
            case 3:
                now_serifu = 27;
                break;
            case 4:
                goto_sub_sub_step(30);
                break;
            }
        } else {
            sel = get_g_zyotai_low(3);
            switch (sel) {
            case 0:
            case 1:
                now_serifu = 154;
                break;
            case 2:
                now_serifu = 156;
                break;
            case 3:
                now_serifu = 158;
                break;
            case 4:
                goto_sub_sub_step(30);
                break;
            }
            if ((s32) (*(s32 *) &girl3_flag << 29) >= 0) {
                now_serifu = (s16) (now_serifu + 6);
            }
            if (((u32) girl3_basyo >> 4) == 5) {
                now_serifu = now_serifu + 12;
            }
        }
        break;
    case 27:
        serifu_run();
        break;
    case 28:
        fade_down_main();
        break;
    case 29:
        serifu_run();
        break;
    case 30:
        fade_out_step();
        break;
    case 31:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 32:
        serifu_run();
        break;
    case 33:
        bunkaken_flags |= 1;
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
