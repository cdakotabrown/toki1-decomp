/* Address: 0x8014918C */
/* Size: 3976 bytes (994 insns) */
/* @SymbolStart: 0x80149184 */
/* @SymbolSize: 3984 */
/* @RodataBase: 0x80155094 */
/* @PostProc: patch_insn_reg_field 0x17c:rt:t8:t7 0x194:rs:t8:t7 0x194:rt:t8:t7 0x198:rt:t7:t8 0x3f4:rt:v0:v1 0x3f8:rs:v0:v1 0x3f8:rt:v0:v1 0x400:rs:v0:v1 0x40c:rs:v0:v1 0x418:rs:v0:v1 0x848:rt:v0:v1 0x84c:rs:v0:v1 0x84c:rt:v0:v1 0x854:rs:v0:v1 0x860:rs:v0:v1 0x86c:rs:v0:v1 0xb90:rt:v0:v1 0xb94:rs:v0:v1 0xb94:rt:v0:v1 0xb9c:rs:v0:v1 0xdf0:rt:v0:v1 0xdf4:rs:v0:v1 0xdf4:rt:v0:v1 0xdfc:rs:v0:v1 0xe08:rs:v0:v1 0xe14:rs:v0:v1 */
#include "types.h"

extern u8 now_sub_sub_step;

extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32);
extern void device_cmd_send(s32);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void k_reset(s32);
extern void set_dec_bri(s32);
extern void dec_bg_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern void bustup_cd_read(s32);
extern void bustup_kuchi_ani(void);
extern void basyo_init(s32);
extern void mes_window_switch(s32);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void menu3_main(void);
extern void menu2_main(void);
extern void bustup_face_change(s32);
extern void zyotai_sprite_set_chk(void);
extern void date_name_set(s32);
extern void serifu_run(void);
extern void event_log_add(s32, s32);
extern s32 get_g_zyotai_low(s32);
extern void Vblnk_Timer_Init(void);
extern s32 Vblnk_Timer(void);
extern void bunkaken_birth_bank_set(void);
extern void bunkaken_sub_reset(void);
extern void bunkaken_ptr_init(void);
extern void bunkaken_msg_set(void);

extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern u8  now_year;
extern u8  now_month;
extern u8  now_day;
extern s32 p_birthday;
extern volatile s16 girl0_tokimeki;
extern volatile s16 girl0_yuukou;
extern volatile s16 girl0_syoushin;
extern u8  girl0_basyo;
extern u8  day_flag;
extern u8  now_girl;
extern s32 serifu_no;
extern s16 mes_base_y;
extern u8  bunkaken_state9_str0[];
extern u8  bunkaken_state9_str1[];
extern u8  bunkaken_state9_str2[];
extern u8  bunkaken_state9_str3[];
extern s32 bunkaken_bank_ptr[];
extern s32 bunkaken_serifu_bank_tbl[][13];

void bunkaken_birth_main(void) {
    u8 idx = now_sub_sub_step;
    s32 v0;
    s32 v1;

    switch (idx) {
    case 0:
        day_flag |= 1;
        k_reset(0);
        basyo_init((s32)bunkaken_state9_str0);
        mes_window_switch(0);
        dec_bg_reset();
        set_dec_bri(0);
        device_cmd_send(207);
        cd_read_start2(3, 0x801E8000, 18871);
        Vblnk_Timer_Init();
        sub_sub_step_up();
        break;
    case 1:
        v0 = Vblnk_Timer();
        if ((u32)v0 < 21) {
            break;
        }
        if (get_CD_State2() & 1) {
            device_load_start(1, 50310, 50265, 51875, 51805, 51788);
            now_girl = 0;
            date_name_set(0);
            bunkaken_birth_bank_set();
            now_serifu = 0;
            now_serifu_in = 0;
            ss_serifu = bunkaken_bank_ptr[0];
            ss_serifu_kind = bunkaken_bank_ptr[1];
            sss_sound = bunkaken_bank_ptr[2];
            sub_sub_step_up();
        }
        break;
    case 2:
        if (((u32)girl0_basyo >> 4) < 6) {
            goto_sub_sub_step(46);
            break;
        }

        v0 = p_birthday;
        v1 = now_day;
        if (now_month == (v0 & 0xF) &&
            v1 == ((u32)(v0 << 23) >> 27) &&
            (u32)get_g_zyotai_low(0) < 2) {
            goto_sub_sub_step(22);
        } else {
            sub_sub_step_up();
        }
        break;
    case 3:
        if (device_load_poll() == 1) {
            now_serifu = 37;
            if ((u32)get_g_zyotai_low(0) < 2) {
                value_cache_find_or_load(17112, 1);
            } else {
                value_cache_find_or_load(16443, 1);
            }
            sub_sub_step_up();
        }
        break;
    case 4:
        serifu_run();
        break;
    case 5:
        if (get_CD_State2() & 1) {
            device_cmd_send(512);
            k_reset(0);
            if ((u32)get_g_zyotai_low(0) < 2) {
                basyo_init((s32)bunkaken_state9_str1);
                event_log_add(20, 25);
            } else {
                basyo_init((s32)bunkaken_state9_str2);
                event_log_add(20, 26);
            }
            mes_base_y = 192;
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 6:
        fade_in();
        break;
    case 7:
        now_serifu = 40;
        sub_sub_step_up();
        break;
    case 8:
        serifu_run();
        break;
    case 9:
        now_serifu = now_year - 92;
        sub_sub_step_up();
        break;
    case 10:
        serifu_run();
        break;
    case 11:
        menu3_main();
        break;
    case 12:
        v0 = get_g_zyotai_low(0);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 23;
            break;
        case 2:
            now_serifu = 24;
            break;
        case 3:
            now_serifu = 25;
            break;
        case 4:
            now_serifu = 26;
            break;
        }
        sub_sub_step_up();
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        switch (serifu_no) {
        case 0:
            now_serifu = 27;
            bustup_face_change(1);
            girl0_tokimeki = girl0_tokimeki + 3;
            girl0_yuukou = girl0_yuukou + 2;
            girl0_syoushin = girl0_syoushin - 15;
            break;
        case 1:
            now_serifu = 29;
            bustup_face_change(2);
            girl0_tokimeki = girl0_tokimeki + 2;
            girl0_yuukou = girl0_yuukou + 1;
            girl0_syoushin = girl0_syoushin - 10;
            break;
        case 2:
            now_serifu = 31;
            bustup_face_change(3);
            girl0_yuukou = girl0_yuukou + 1;
            girl0_syoushin = girl0_syoushin + 10;
            break;
        }
        bustup_kuchi_ani();
        sub_sub_step_up();
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        if ((u32)get_g_zyotai_low(0) < 2) {
            now_serifu = 39;
        }
        sub_sub_step_up();
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        fade_out();
        break;
    case 19:
        sub_sub_step_up();

    case 20:
        sub_sub_step_up();
        break;
    case 21:
        bunkaken_sub_reset();
        break;
    case 22:
        if (device_load_poll() == 1) {
            day_flag |= 4;
            now_serifu = 37;
            value_cache_find_or_load(17112, 1);
            sub_sub_step_up();
        }
        break;
    case 23:
        serifu_run();
        break;
    case 24:
        if (get_CD_State2() & 1) {
            event_log_add(20, 25);
            device_cmd_send(512);
            k_reset(0);
            basyo_init((s32)bunkaken_state9_str3);
            mes_base_y = 192;
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            cd_read_start2(3, 0x801B0000, 44867);
            sub_sub_step_up();
        }
        break;
    case 25:
        fade_in();
        break;
    case 26:
        if (get_CD_State2() & 1) {
            bunkaken_ptr_init();
            now_serifu = 1;
            now_serifu_in = 0;
            ss_serifu = bunkaken_serifu_bank_tbl[0][1];
            ss_serifu_kind = bunkaken_serifu_bank_tbl[1][1];
            sss_sound = bunkaken_serifu_bank_tbl[2][1];
            sub_sub_step_up();
        }
        break;
    case 27:
        serifu_run();
        break;
    case 28:
        cd_read_start2(3, 0x801E8000, 18871);
        sub_sub_step_up();
        break;
    case 29:
        if (get_CD_State2() & 1) {
            bunkaken_birth_bank_set();
            now_serifu = 38;
            now_serifu_in = 0;
            ss_serifu = bunkaken_bank_ptr[0];
            ss_serifu_kind = bunkaken_bank_ptr[1];
            sss_sound = bunkaken_bank_ptr[2];
            sub_sub_step_up();
        }
        break;
    case 30:
        serifu_run();
        break;
    case 31:
        now_serifu = 41;
        sub_sub_step_up();
        break;
    case 32:
        serifu_run();
        break;
    case 33:
        now_serifu = now_year - 92;
        sub_sub_step_up();
        break;
    case 34:
        serifu_run();
        break;
    case 35:
        menu3_main();
        break;
    case 36:
        now_serifu = 35;
        sub_sub_step_up();
        break;
    case 37:
        serifu_run();
        break;
    case 38:
        sub_sub_step_up();
        break;
    case 39:
        sub_sub_step_up();
        break;
    case 40:
        switch (serifu_no) {
        case 0:
            now_serifu = 27;
            bustup_face_change(1);
            girl0_tokimeki = girl0_tokimeki + 3;
            girl0_yuukou = girl0_yuukou + 2;
            girl0_syoushin = girl0_syoushin - 15;
            break;
        case 1:
            now_serifu = 29;
            bustup_face_change(2);
            girl0_tokimeki = girl0_tokimeki + 2;
            girl0_yuukou = girl0_yuukou + 1;
            girl0_syoushin = girl0_syoushin - 10;
            break;
        case 2:
            now_serifu = 31;
            bustup_face_change(3);
            girl0_yuukou = girl0_yuukou + 1;
            girl0_syoushin = girl0_syoushin + 10;
            break;
        }
        bustup_kuchi_ani();
        sub_sub_step_up();
        break;
    case 41:
        serifu_run();
        break;
    case 42:
        if ((u32)get_g_zyotai_low(0) < 2) {
            now_serifu = 39;
        }
        sub_sub_step_up();
        break;
    case 43:
        serifu_run();
        break;
    case 44:
        fade_out();
        break;
    case 45:
        bunkaken_sub_reset();
        break;
    case 46:
        if (device_load_poll() == 1) {
            now_serifu = 0;
            switch ((u32)girl0_basyo >> 4) {
            case 0:
                if (now_year == 97) {
                    value_cache_find_or_load(16628, 1);
                } else {
                    value_cache_find_or_load(16618, 1);
                }
                break;
            case 1:
            case 5:
                value_cache_find_or_load(16628, 1);
                break;
            case 2:
                value_cache_find_or_load(16636, 1);
                break;
            case 3:
                value_cache_find_or_load(16645, 1);
                break;
            case 4:
                if (now_year == 96) {
                    value_cache_find_or_load(16664, 1);
                } else {
                    value_cache_find_or_load(16655, 1);
                }
                break;
            }
            sub_sub_step_up();
        }
        break;
    case 47:
        serifu_run();
        break;
    case 48:
        if (get_CD_State2() & 1) {
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            switch ((u32)girl0_basyo >> 4) {
            case 1:
                bustup_cd_read(21623);
                break;
            case 2:
                bustup_cd_read(21713);
                break;
            case 4:
                bustup_cd_read(21668);
                break;
            default:
                bustup_cd_read(21578);
                break;
            }
            sub_sub_step_up();
        }
        break;
    case 49:
        menu2_main();
        break;
    case 50:
        if (serifu_no == 0) {
            goto_sub_sub_step(52);
        } else {
            serifu_run();
        }
        break;
    case 51:
        girl0_yuukou = girl0_yuukou - 1;
        girl0_syoushin = girl0_syoushin + 5;
        bunkaken_sub_reset();
        break;
    case 52:
        device_cmd_send(512);
        now_serifu = now_year - 92;
        sub_sub_step_up();
        break;
    case 53:
        serifu_run();
        break;
    case 54:
        menu3_main();
        break;
    case 55:
        if (get_CD_State2() & 1) {
            now_serifu = 6;
            bunkaken_msg_set();
            bustup_ptr_init();
            now_girl = 0;
            date_name_set(0);
            zyotai_sprite_set_chk();
        }
        break;
    case 56:
        serifu_run();
        break;
    case 57:
        fade_in_step();
        break;
    case 58:
        now_serifu = 13;
        sub_sub_step_up();
        break;
    case 59:
        serifu_run();
        break;
    case 60:
        fade_up_main();
        break;
    case 61:
        v0 = get_g_zyotai_low(0);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 18;
            break;
        case 2:
            now_serifu = 19;
            break;
        case 3:
            now_serifu = 20;
            break;
        case 4:
            now_serifu = 21;
            break;
        }
        sub_sub_step_up();
        break;
    case 62:
        serifu_run();
        break;
    case 63:
        v0 = get_g_zyotai_low(0);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 23;
            break;
        case 2:
            now_serifu = 24;
            break;
        case 3:
            now_serifu = 25;
            break;
        case 4:
            now_serifu = 26;
            break;
        }
        sub_sub_step_up();
        break;
    case 64:
        serifu_run();
        break;
    case 65:
        switch (serifu_no) {
        case 0:
            now_serifu = 27;
            bustup_face_change(1);
            girl0_tokimeki = girl0_tokimeki + 3;
            girl0_yuukou = girl0_yuukou + 2;
            girl0_syoushin = girl0_syoushin - 15;
            break;
        case 1:
            now_serifu = 29;
            bustup_face_change(2);
            girl0_tokimeki = girl0_tokimeki + 2;
            girl0_yuukou = girl0_yuukou + 1;
            girl0_syoushin = girl0_syoushin - 10;
            break;
        case 2:
            now_serifu = 31;
            bustup_face_change(3);
            girl0_yuukou = girl0_yuukou + 1;
            girl0_syoushin = girl0_syoushin + 10;
            break;
        }
        bustup_kuchi_ani();
        sub_sub_step_up();
        break;
    case 66:
        serifu_run();
        break;
    case 67:
        fade_down_main();
        break;
    case 68:
        serifu_run();
        break;
    case 69:
        fade_out_step();
        break;
    case 70:
        now_serifu = 36;
        sub_sub_step_up();
        break;
    case 71:
        serifu_run();
        break;
    case 72:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
