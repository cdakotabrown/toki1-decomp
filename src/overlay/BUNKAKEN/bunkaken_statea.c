/* @SymbolStart: 0x8014A114 */
/* @RodataBase: 0x8015520C */
/* @SymbolSize: 1308 */
/* @PostProc: patch_insn_reg_field 0x110:rt:a0:v1 0x114:rs:a0:v1 0x294:rt:a0:v1 0x298:rs:a0:v1 0x2cc:rt:a0:v1 0x2d8:rs:a0:v1 0x2d8:rt:a0:v1 0x2e8:rs:a0:v1 */
/* Address: 0x8014A11C */
/* Size: 1300 bytes (325 insns) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 device_cmd_send(s32);
extern s32 device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);
extern s32 get_CD_State2(void);
extern s32 cd_read_start2(s32, s32, s32);
extern s32 game_reset(void);
extern s32 k_reset(s32);
extern s32 get_g_zyotai_h(s32);
extern s32 set_dec_bri(s32);
extern s32 dec_bg_reset(void);
extern s32 indexed_byte_lsb_set(s32, s32);
extern s32 value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern s32 mes_window_switch(s32);
extern s32 basyo_init(s32);
extern s32 fade_in(void);
extern s32 fade_out(void);
extern s32 bustup_face_change(s32);
extern s32 date_name_set(s32);
extern s32 serifu_run(void);
extern s32 event_log_add(s32, s32);
extern s32 Vblnk_Timer_Init(void);
extern s32 Vblnk_Timer(void);
extern s32 bunkaken_birth_bank_set(void);
extern s32 bunkaken_sub_reset(void);
extern s32 bunkaken_ptr_init(void);

extern u8 day_flag;
extern u8 now_girl;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern s32 bunka_girl;
extern s16 mes_base_y;
extern u8 bunkaken_statea_str0[];
extern u8 bunkaken_statea_str1[];
extern s32 bunkaken_bank_ptr[];
extern s32 bunkaken_serifu_bank_tbl[][13];
extern u8 now_sub_sub_step;

void bunkaken_statea(void) {
    s32 t6 = now_sub_sub_step;
    s32 v0;
    s32 v1;
    s32 a0;

    switch (t6) {
    case 0:
        sub_sub_step_up();
        break;
    case 1:
        k_reset(0);
        basyo_init((s32)bunkaken_statea_str0);
        mes_window_switch(0);
        dec_bg_reset();
        set_dec_bri(0);
        day_flag = day_flag | 4;
        device_cmd_send(207);
        Vblnk_Timer_Init();
        cd_read_start2(3, 0x801E8000, 18871);
        sub_sub_step_up();
        break;
    case 2:
        v0 = Vblnk_Timer();
        if ((u32)v0 < 21) {
            break;
        }
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        device_load_start(1, 0xC486, 0xC459, 0xCAA3, 0xCA5D, 0xCA4C);
        v1 = bunka_girl;
        if (v1 == 11) {
            now_girl = 0;
            date_name_set(0);
        } else {
            now_girl = (u8)v1;
            date_name_set(v1 & 0xFF);
        }
        bunkaken_birth_bank_set();
        now_serifu = 37;
        now_serifu_in = 0;
        ss_serifu = bunkaken_bank_ptr[0];
        ss_serifu_kind = bunkaken_bank_ptr[1];
        sss_sound = bunkaken_bank_ptr[2];
        sub_sub_step_up();
        break;
    case 3:
        serifu_run();
        break;
    case 4:
        v0 = device_load_poll();
        if (v0 != 1) {
            break;
        }
        value_cache_find_or_load(17112, 1);
        sub_sub_step_up();
        break;
    case 5:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        a0 = now_girl;
        v0 = get_g_zyotai_h(a0);
        bustup_face_change((u8)v0 & 0x7F);
        device_cmd_send(512);
        k_reset(0);
        basyo_init((s32)bunkaken_statea_str1);
        mes_base_y = 192;
        v0 = dec_bg_init(1);
        indexed_byte_lsb_set(1, v0);
        cd_read_start2(3, 0x801B0000, 0xAF43);
        sub_sub_step_up();
        break;
    case 6:
        fade_in();
        break;
    case 7:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        bunkaken_ptr_init();
        v1 = bunka_girl;
        if (v1 == 11) {
            now_girl = 0;
            date_name_set(0);
        } else {
            now_girl = (u8)v1;
            date_name_set(v1 & 0xFF);
        }
        v1 = bunka_girl;
        now_serifu = 1;
        now_serifu_in = 0;
        switch (v1 - 4) {
        case 7:
            ss_serifu = bunkaken_serifu_bank_tbl[0][1];
            ss_serifu_kind = bunkaken_serifu_bank_tbl[1][1];
            sss_sound = bunkaken_serifu_bank_tbl[2][1];
            break;
        case 0:
            ss_serifu = bunkaken_serifu_bank_tbl[0][10];
            ss_serifu_kind = bunkaken_serifu_bank_tbl[1][10];
            sss_sound = bunkaken_serifu_bank_tbl[2][10];
            break;
        case 1:
            ss_serifu = bunkaken_serifu_bank_tbl[0][7];
            ss_serifu_kind = bunkaken_serifu_bank_tbl[1][7];
            sss_sound = bunkaken_serifu_bank_tbl[2][7];
            break;
        case 2:
            ss_serifu = bunkaken_serifu_bank_tbl[0][8];
            ss_serifu_kind = bunkaken_serifu_bank_tbl[1][8];
            sss_sound = bunkaken_serifu_bank_tbl[2][8];
            break;
        case 3:
            ss_serifu = bunkaken_serifu_bank_tbl[0][5];
            ss_serifu_kind = bunkaken_serifu_bank_tbl[1][5];
            sss_sound = bunkaken_serifu_bank_tbl[2][5];
            break;
        case 4:
            ss_serifu = bunkaken_serifu_bank_tbl[0][0];
            ss_serifu_kind = bunkaken_serifu_bank_tbl[1][0];
            sss_sound = bunkaken_serifu_bank_tbl[2][0];
            break;
        case 5:
            ss_serifu = bunkaken_serifu_bank_tbl[0][9];
            ss_serifu_kind = bunkaken_serifu_bank_tbl[1][9];
            sss_sound = bunkaken_serifu_bank_tbl[2][9];
            break;
        case 6:
            ss_serifu = bunkaken_serifu_bank_tbl[0][12];
            ss_serifu_kind = bunkaken_serifu_bank_tbl[1][12];
            sss_sound = bunkaken_serifu_bank_tbl[2][12];
            break;
        default:
            game_reset();
            break;
        }
        sub_sub_step_up();
        break;
    case 8:
        serifu_run();
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        fade_out();
        break;
    case 11:
        event_log_add(20, 25);
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
