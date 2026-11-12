/* @SymbolStart: 0x80147214 */
/* @RodataBase: 0x8015DE94 */
/* Address: 0x8014721C */
/* Size: 3440 bytes (860 insns) */
/* @SymbolSize: 3448 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x200:rt:v0:v1 0x204:rs:v0:v1 0x204:rt:v0:v1 0x20c:rs:v0:v1 0x77c:rt:v0:v1 0x780:rs:v0:v1 0x780:rt:v0:v1 0x788:rs:v0:v1 0x794:rs:v0:v1 0x7a0:rs:v0:v1 0xb9c:rt:v0:v1 0xba0:rs:v0:v1 0xba0:rt:v0:v1 0xba8:rs:v0:v1 0xbb4:rs:v0:v1 0xbc0:rs:v0:v1 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32 a0);
extern void device_cmd_send(s32 a0);
extern void device_load_start(s32 a0, s32 a1, s32 a2, s32 a3, s32 arg4, s32 arg5);
extern s32  device_load_poll(void);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void game_reset(void);
extern void k_reset(s32 a0);
extern void set_dec_bri(s32 a0);
extern void dec_bg_reset(void);
extern void indexed_byte_lsb_set(s32 a0, s32 a1);
extern void value_cache_find_or_load(s32 a0, s32 a1);
extern s32  dec_bg_init(s32 a0);
extern void bustup_cd_read(s32 a0);
extern void bustup_kuchi_ani(void);
extern void mes_window_switch(s32 a0);
extern void basyo_init(void *a0);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_out_step(void);
extern void menu3_main(void);
extern void menu2_main(void);
extern void bustup_face_change(s32 a0);
extern void zyotai_sprite_set_chk(void);
extern void date_name_set(s32 a0);
extern void serifu_run(void);
extern void event_log_add(s32 a0, s32 a1);
extern void Vblnk_Timer_Init(void);
extern s32  Vblnk_Timer(void);
extern s32  get_g_zyotai_low(s32 a0);
extern void bunkasai_birth_bank_set(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_ptr_init(void);
extern void bunkasai_msg_set(void);

extern s32  bunka_girl;
extern s32  serifu_no;

extern s16  now_serifu;
extern s16  now_serifu_in;
extern s32  ss_serifu;
extern s32  ss_serifu_kind;
extern s32  sss_sound;

extern u8   day_flag;
extern u8   now_girl;

extern s32  p_birthday;
extern u8   now_month;
extern u8   now_day;
extern u8   now_year;

extern u8   girl0_basyo;
extern u16  basyo_pack;

extern s16  girl0_tokimeki;
extern s16  girl0_yuukou;
extern s16  girl0_syoushin;

extern s32 bunkasai_serifu_ptr[];

extern char bunkasai_birth_main_str0[];

extern u8 now_sub_sub_step;
extern s32 bunkasai_serifu_bank_tbl[][13];
void bunkasai_birth_main(void) {
    s32 dispatch = now_sub_sub_step;

    s32 v0;
    s32 v1;

    switch (dispatch) {
    case 0:
        k_reset(0);
        basyo_init(bunkasai_birth_main_str0);
        mes_window_switch(0);
        set_dec_bri(0);
        day_flag |= 1;
        device_cmd_send(207);
        Vblnk_Timer_Init();
        cd_read_start2(3, 0x801E8000, 18871);
        sub_sub_step_up();
        break;
    case 1:
        v0 = Vblnk_Timer();
        if ((u32)v0 < 21) {
            break;
        }
        if (get_CD_State2() & 1) {
            device_load_start(1, 0xC486, 0xC459, 0xCAA3, 0xCA5D, 0xCA4C);
            now_girl = 0;
            date_name_set(0);
            bunkasai_birth_bank_set();
            now_serifu = 0;
            now_serifu_in = 0;
            ss_serifu = bunkasai_serifu_ptr[0];
            ss_serifu_kind = bunkasai_serifu_ptr[1];
            sss_sound = bunkasai_serifu_ptr[2];
            sub_sub_step_up();
        }
        break;
    case 2:
        v0 = p_birthday;
        if ((v0 & 0xF) == now_month &&
            (s32)(((u32)v0 << 23) >> 27) == now_day) {
            v0 = get_g_zyotai_low(0);
            if ((u32)v0 < 2) {
                v0 = bunka_girl;
                if (v0 == 0) {
                    goto_sub_sub_step(36);
                    break;
                }
            }
        }
        dec_bg_reset();
        set_dec_bri(0);
        sub_sub_step_up();
        break;
    case 3:
        serifu_run();
        break;
    case 4:
        menu2_main();
        break;
    case 5:
        v1 = serifu_no;
        if (v1 != 0) {
            serifu_run();
        } else {
            goto_sub_sub_step(7);
        }
        break;
    case 6:
        girl0_yuukou = (s16)(girl0_yuukou - 1);
        girl0_syoushin = (s16)(girl0_syoushin + 5);
        bunkasai_sub_reset();
        break;
    case 7: {
        s32 inner;
        if (device_load_poll() != 1) {
            break;
        }
        device_cmd_send(512);
        inner = (girl0_basyo >> 4);
        switch (inner) {
        case 0:
            if (now_year == 97) {
                event_log_add(20, 28);
                value_cache_find_or_load(16628, 1);
            } else {
                event_log_add(20, 27);
                value_cache_find_or_load(16618, 1);
            }
            break;
        case 1:
            event_log_add(20, 29);
            value_cache_find_or_load(16628, 1);
            break;
        case 5:
            event_log_add(20, 34);
            value_cache_find_or_load(16628, 1);
            break;
        case 2:
            event_log_add(20, 30);
            value_cache_find_or_load(16636, 1);
            break;
        case 3:
            event_log_add(20, 31);
            value_cache_find_or_load(16645, 1);
            break;
        case 4:
            if (now_year == 96) {
                event_log_add(20, 33);
                value_cache_find_or_load(16664, 1);
            } else {
                event_log_add(20, 32);
                value_cache_find_or_load(16655, 1);
            }
            break;
        default:
            value_cache_find_or_load(16443, 1);
            break;
        }
        now_serifu = 2;
        sub_sub_step_up();
        break;
    }
    case 8:
        serifu_run();
        break;
    case 9:
        if (get_CD_State2() & 1) {
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            v0 = (u32)girl0_basyo >> 4;
            switch (v0) {
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
    case 10:
        now_serifu = (s16)((s32)now_year - 92);
        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        menu3_main();
        break;
    case 13:
        bunkasai_msg_set();
        now_serifu = 6;
        sub_sub_step_up();
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            zyotai_sprite_set_chk();
        }
        break;
    case 16: {
        s32 a;
        s32 b;
        a = ((u32)basyo_pack >> 12);
        b = ((u32)girl0_basyo >> 4);
        if (b == a) {
            now_serifu = 10;
        } else {
            now_serifu = 11;
        }
        sub_sub_step_up();
        break;
    }
    case 17:
        serifu_run();
        break;
    case 18:
        fade_in_step();
        break;
    case 19: {
        s32 a;
        s32 b;
        a = ((u32)basyo_pack >> 12);
        b = ((u32)girl0_basyo >> 4);
        if (b == a) {
            now_serifu = 14;
        } else {
            now_serifu = 13;
        }
        sub_sub_step_up();
        break;
    }
    case 20:
        serifu_run();
        break;
    case 21:
        fade_up_main();
        break;
    case 22:
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
    case 23:
        serifu_run();
        break;
    case 24: {
        s32 a;
        s32 b;
        a = ((u32)basyo_pack >> 12);
        b = ((u32)girl0_basyo >> 4);
        if (b == a) {
            now_serifu = 22;
            sub_sub_step_up();
        } else {
            goto_sub_sub_step(26);
        }
        break;
    }
    case 25:
        serifu_run();
        break;
    case 26:
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
    case 27:
        serifu_run();
        break;
    case 28:
        v1 = serifu_no;
        switch (v1) {
        case 0:
            now_serifu = 27;
            bustup_face_change(1);
            girl0_tokimeki = (s16)(girl0_tokimeki + 3);
            girl0_yuukou = (s16)(girl0_yuukou + 2);
            girl0_syoushin = (s16)(girl0_syoushin - 15);
            break;
        case 1:
            now_serifu = 29;
            bustup_face_change(2);
            girl0_tokimeki = (s16)(girl0_tokimeki + 2);
            girl0_yuukou = (s16)(girl0_yuukou + 1);
            girl0_syoushin = (s16)(girl0_syoushin - 10);
            break;
        case 2:
            now_serifu = 31;
            bustup_face_change(3);
            girl0_yuukou = (s16)(girl0_yuukou + 1);
            girl0_syoushin = (s16)(girl0_syoushin + 10);
            break;
        }
        bustup_kuchi_ani();
        sub_sub_step_up();
        break;
    case 29:
        serifu_run();
        break;
    case 30:
        fade_down_main();
        break;
    case 31:
        serifu_run();
        break;
    case 32:
        fade_out_step();
        break;
    case 33:
        now_serifu = 36;
        sub_sub_step_up();
        break;
    case 34:
        serifu_run();
        break;
    case 35:
        bunkasai_sub_reset();
        break;
    case 36:
        if (device_load_poll() != 1) {
            break;
        }
        device_cmd_send(512);
        day_flag |= 4;
        sub_sub_step_up();
        break;
    case 37:
        fade_in_step();
        break;
    case 38: {
        s32 a;
        s32 b;
        a = ((u32)basyo_pack >> 12);
        b = ((u32)girl0_basyo >> 4);
        if (b != a) {
            now_serifu = 34;
            goto_sub_sub_step(62);
        } else {
            now_serifu = 42;
            sub_sub_step_up();
        }
        break;
    }
    case 39:
        fade_up_main();
        break;
    case 40:
        serifu_run();
        break;
    case 41:
        cd_read_start2(3, 0x801B0000, 0xAF43);
        sub_sub_step_up();
        break;
    case 42:
        if (get_CD_State2() & 1) {
            bunkasai_ptr_init();
            now_serifu = 1;
            now_serifu_in = 0;
            ss_serifu = bunkasai_serifu_bank_tbl[0][1];
            ss_serifu_kind = bunkasai_serifu_bank_tbl[1][1];
            sss_sound = bunkasai_serifu_bank_tbl[2][1];
            sub_sub_step_up();
        }
        break;
    case 43:
        serifu_run();
        break;
    case 44:
        cd_read_start2(3, 0x801E8000, 18871);
        sub_sub_step_up();
        break;
    case 45:
        if (get_CD_State2() & 1) {
            bunkasai_birth_bank_set();
            now_serifu = 38;
            now_serifu_in = 0;
            ss_serifu = bunkasai_serifu_ptr[0];
            ss_serifu_kind = bunkasai_serifu_ptr[1];
            sss_sound = bunkasai_serifu_ptr[2];
            sub_sub_step_up();
        }
        break;
    case 46:
        serifu_run();
        break;
    case 47:
        now_serifu = 41;
        sub_sub_step_up();
        break;
    case 48:
        serifu_run();
        break;
    case 49:
        now_serifu = (s16)((s32)now_year - 92);
        sub_sub_step_up();
        break;
    case 50:
        serifu_run();
        break;
    case 51:
        menu3_main();
        break;
    case 52:
        now_serifu = 35;
        sub_sub_step_up();
        break;
    case 53:
        serifu_run();
        break;
    case 54:
        v1 = serifu_no;
        switch (v1) {
        case 0:
            now_serifu = 27;
            bustup_face_change(1);
            girl0_tokimeki = (s16)(girl0_tokimeki + 3);
            girl0_yuukou = (s16)(girl0_yuukou + 2);
            girl0_syoushin = (s16)(girl0_syoushin - 15);
            break;
        case 1:
            now_serifu = 29;
            bustup_face_change(2);
            girl0_tokimeki = (s16)(girl0_tokimeki + 2);
            girl0_yuukou = (s16)(girl0_yuukou + 1);
            girl0_syoushin = (s16)(girl0_syoushin - 10);
            break;
        case 2:
            now_serifu = 31;
            bustup_face_change(3);
            girl0_yuukou = (s16)(girl0_yuukou + 1);
            girl0_syoushin = (s16)(girl0_syoushin + 10);
            break;
        }
        bustup_kuchi_ani();
        sub_sub_step_up();
        break;
    case 55:
        serifu_run();
        break;
    case 56:
        fade_down_main();
        break;
    case 57:
        serifu_run();
        break;
    case 58:
        fade_out_step();
        break;
    case 59:
        now_serifu = 36;
        sub_sub_step_up();
        break;
    case 60:
        serifu_run();
        break;
    case 61:
        bunkasai_sub_reset();
        break;
    case 62:
        fade_up_main();
        break;
    case 63:
        serifu_run();
        break;
    case 64:
        now_serifu = 38;
        goto_sub_sub_step(46);
        break;
    default:
        game_reset();
        break;
    }
}
