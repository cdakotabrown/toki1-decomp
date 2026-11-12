/* @SymbolStart: 0x80132EC8 */
/* @SymbolSize: 844 */
/* @RodataBase: 0x80134198 */
/* @LiteralBase: 0x801340DC */
/* rensyu_load3 - RENSYU overlay, Address: 0x80132ED0, Size: 836 bytes (209 insns) */
#include "types.h"
#include "overlay_text/RENSYU.h"

extern u8 now_sub_sub_step;

extern void device_cmd_send(u32 a0);
extern void device_load_start(u32 a0, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5);
extern void date_x_taku_set(u32 a0, u32 a1);
extern void shared_system_state_reset(void);
extern void message_window_init(void);
extern void icon_disp_switch(u32 a0);
extern void mascot_disp_switch(u32 a0);
extern void parameter_disp_switch(u32 a0);
extern void hizuke_init(void);
extern void k_reset(u32 a0);
extern void basyo_init(u32 a0);
extern void mes_window_switch(u32 a0);
extern void dec_bg_reset(void);
extern void set_dec_bri(u32 a0);
extern void srand(u32 a0);
extern void sub_sub_step_up(void);
extern u32  device_load_poll(void);
extern void cd_read_start2(u32 a0, u32 a1, u32 a2);
extern u32  get_CD_State2(void);
extern void date_name_set(u32 a0);
extern void yazirushi_off(void);
extern void rensyu_serifu_tbl_set(void);
extern void game_reset(void);
extern void common_disp(void);
extern void serifu_run(void);
extern void sub_step_up(void);
extern void value_cache_find_or_load(u32 a0, u32 a1);
extern u32  dec_bg_init(u32 a0);
extern void indexed_byte_lsb_set(u32 a0, u32 a1);
extern void fade_in_step(void);
extern void set_kanji_string(s32 a0, u32 a1, u32 a2, u32 a3, u32 a4);
extern void k_disp_start(u32 a0);
extern void rensyu_basyo6_res_set(void);
extern void rensyu_basyo7_res_set(void);
extern void rensyu_basyo8_res_set(void);
extern void rensyu_basyo9_res_set(void);
extern void rensyu_basyo10_res_set(void);
extern void palette_upload_enqueue(u32 a0, u32 a1, u32 a2, u32 a3, u32 a4);

extern u32 rensyu_serifu_ptr;
extern u32 rensyu_serifu_kind_ptr;
extern u32 rensyu_sound_ptr;
extern u16 basyo_pack;
extern u32 rensyu_res_pal_ptr;

void rensyu_load3(void)
{
    u32 t6 = now_sub_sub_step;

    u16 val;
    u32 idx;
    s32 s0;
    u32 a0_val;

    switch (t6) {
    case 0:
        val = basyo_pack;
        idx = ((u32)val >> 12) - 6;
        switch (idx) {
        case 0:
            value_cache_find_or_load(16762, 1);
            break;
        case 1:
            value_cache_find_or_load(16771, 1);
            break;
        case 2:
            value_cache_find_or_load(16798, 1);
            break;
        case 3:
            value_cache_find_or_load(16789, 1);
            break;
        case 4:
            value_cache_find_or_load(16781, 1);
            break;
        default:
            game_reset();
            break;
        }
        sub_sub_step_up();
        break;
    case 1:
        if (get_CD_State2() & 1) {
            a0_val = dec_bg_init(1);
            indexed_byte_lsb_set(1, a0_val);

            val = basyo_pack;
            idx = ((u32)val >> 12) - 6;
            switch (idx) {
            case 0:
                cd_read_start2(27, 0x80197000, 0xB03E);
                break;
            case 1:
                cd_read_start2(26, 0x80197000, 0xAFE7);
                break;
            case 2:
                cd_read_start2(27, 0x80197000, 0xB01C);
                break;
            case 3:
                cd_read_start2(27, 0x80197000, 0xB001);
                break;
            case 4:
                cd_read_start2(27, 0x80197000, 0xAFCC);
                break;
            default:
                game_reset();
                break;
            }
            mes_window_switch(1);
            sub_sub_step_up();
        }
        break;
    case 2:
        fade_in_step();
        break;
    case 3:
        k_reset(1);
        set_kanji_string(-128, 50, 0, (s32)TXT_801340DC, 0);
        k_disp_start(1);
        sub_sub_step_up();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            val = basyo_pack;
            idx = ((u32)val >> 12) - 6;
            switch (idx) {
            case 0:
                rensyu_basyo6_res_set();
                s0 = 0;
                break;
            case 1:
                rensyu_basyo7_res_set();
                s0 = 0;
                break;
            case 2:
                rensyu_basyo8_res_set();
                s0 = 0;
                break;
            case 3:
                rensyu_basyo9_res_set();
                s0 = 0;
                break;
            case 4:
                rensyu_basyo10_res_set();
                s0 = 0;
                break;
            default:
                game_reset();
                s0 = 0;
                break;
            }

            do {
                a0_val = rensyu_res_pal_ptr;
                palette_upload_enqueue(a0_val, (u8)(s0 + 16), 1, 1, s0);
                s0++;
            } while (s0 != 5);

            sub_step_up();
        }
        break;
    default:
        game_reset();
        break;
    }

    common_disp();
}
