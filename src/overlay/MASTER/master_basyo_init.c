/* @SymbolStart: 0x801321B0 */
/* @RodataBase: 0x8013BA24 */
/* master_basyo_init - MASTER overlay, Address: 0x801321B8, Size: 652 bytes (163 insns) */
/* @SymbolSize: 660 */
#include "types.h"

extern u16 basyo_pack;
extern u8 master_basyo_init_str0;
extern u8 master_basyo_init_str1;
extern u8 master_basyo_init_str2;
extern u8 master_basyo_init_str3;
extern u8 master_basyo_init_str4;
extern u8 master_basyo_init_str5;

extern void device_cmd_send(s32);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern void date_x_taku_set(s32, s32);
extern void dec_bg_reset(void);
extern void set_dec_bri(s32);
extern void shared_system_state_reset(void);
extern void message_window_init(void);
extern void hizuke_init(void);
extern void parameter_disp_switch(s32);
extern void icon_disp_switch(s32);
extern void mascot_disp_switch(s32);
extern void sub_sub_step_up(void);
extern s32 device_load_poll(void);
extern void k_reset(s32);
extern void basyo_init(void *);
extern void value_cache_find_or_load(s32, s32);
extern void game_reset(void);
extern void mes_window_switch(s32);
extern s32 get_CD_State2(void);
extern s32 dec_bg_init(s32);
extern void indexed_byte_lsb_set(s32, s32);
extern void date_name_set(s32);
extern void yazirushi_off(void);
extern void fade_in_step(void);
extern void sub_step_up(void);
extern void common_disp(void);
extern u8 now_sub_sub_step;

void master_basyo_init(void) {
    s32 sel;
    u32 padidx;
    s32 ret;

    sel = now_sub_sub_step;

    switch (sel) {
    case 0:
        device_cmd_send(191);
        device_load_start(1, 0xC051, 0xC02B, 0xC990, 0xC943, 0xC929);
        date_x_taku_set(1, 0);
        dec_bg_reset();
        set_dec_bri(0);
        shared_system_state_reset();
        message_window_init();
        hizuke_init();
        parameter_disp_switch(0);
        icon_disp_switch(0);
        mascot_disp_switch(0);
        sub_sub_step_up();
        break;
    case 1:
        if (device_load_poll() == 1) {
            k_reset(0);
            padidx = (u32)basyo_pack >> 12;
            switch (padidx) {
            case 0:
                basyo_init(&master_basyo_init_str0);
                value_cache_find_or_load(17988, 1);
                break;
            case 1:
                basyo_init(&master_basyo_init_str1);
                value_cache_find_or_load(17997, 1);
                break;
            case 2:
                basyo_init(&master_basyo_init_str2);
                value_cache_find_or_load(18006, 1);
                break;
            case 3:
                basyo_init(&master_basyo_init_str3);
                value_cache_find_or_load(18015, 1);
                break;
            case 4:
                basyo_init(&master_basyo_init_str4);
                value_cache_find_or_load(18025, 1);
                break;
            case 5:
                basyo_init(&master_basyo_init_str5);
                value_cache_find_or_load(18035, 1);
                break;
            default:
                game_reset();
                break;
            }
            mes_window_switch(0);
            device_cmd_send(514);
            sub_sub_step_up();
        }
        break;
    case 2:
        sub_sub_step_up();
        break;
    case 3:
        if (get_CD_State2() & 1) {
            ret = dec_bg_init(1);
            indexed_byte_lsb_set(1, ret);
            date_name_set(0);
            yazirushi_off();
            mes_window_switch(1);
            sub_sub_step_up();
        }
        break;
    case 4:
        fade_in_step();
        break;
    case 5:
        sub_step_up();
        break;
    default:
        game_reset();
        break;
    }

    common_disp();
}
