/* @SymbolStart: 0x80136CF4 */
/* @SymbolSize: 1088 */
/* @RodataBase: 0x80139500 */
/* @RodataBase: 0x80139500 places both; the linked table words reproduce the */
#include "types.h"

extern u8 now_sub_sub_step;
extern u8 step_mode;
extern u8 screen_bri;
extern s32 vblnk_counter;
extern s32 pad_trig;
extern s32 en_nichi_mod_hdr;
extern s32 en_nichi_pal_ptr;
extern s32 en_nichi_srn_ptr;
extern s32 en_nichi_mod_src;
extern s32 screen_w[];

extern void date_x_taku_set(s32, s32);
extern void set_flag_b3_high_bit(s32);
extern void hizuke_disp_switch(s32);
extern void message_disp_switch(s32);
extern void srand(s32);
extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);
extern void sub_step_up(void);
extern void cd_read_start2(s32, s32, s32);
extern s32 get_CD_State2(void);
extern void en_nichi_init(void);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void mod_init(s32, s32, s32);
extern void srn_init(s32, s32, s32);
extern void srn_vram_set(s32, s32, s32, s32);
extern void fade_timer_init(void);
extern s32 Fade_In_Main(void);
extern void game_reset(void);
extern void Fade_Out_Init(void);
extern s32 Vblnk_Timer(void);
extern void Vblnk_Timer_Init(void);
extern s32 bri_fade_out(void);
extern s32 get_CD_State(void);
extern void set_XAsector(s32, s32, s32);
extern void scroll_poly_disp(s32, s32, s32, s32, s32);

void en_nichi_title_main(void)
{
    u8 step = now_sub_sub_step;
    s32 i;

    switch ((u32)step) {
    case 0:
        date_x_taku_set(1, 0);
        set_flag_b3_high_bit(0);
        hizuke_disp_switch(0);
        message_disp_switch(0);
        srand(vblnk_counter);
        device_cmd_send(207);
        sub_sub_step_up();
        return;

    case 1:
        cd_read_start2(108, 0x801B0000, 32011);
        sub_sub_step_up();
        return;

    case 2:
        if ((get_CD_State2() & 1) == 0) {
            return;
        }
        screen_bri = 0;
        en_nichi_init();
        for (i = 0; i != 2; i++) {
            palette_upload_enqueue(en_nichi_pal_ptr, (u8)(i + 16), 1, 1, i);
        }
        mod_init(0, en_nichi_mod_src, en_nichi_mod_hdr);
        srn_init(0, en_nichi_srn_ptr, 0);
        srn_vram_set(0, 5, 1408, 240);
        fade_timer_init();
        sub_sub_step_up();
        return;

    case 3:
        device_cmd_send(515);
        sub_sub_step_up();
        return;

    case 4:
        if (Fade_In_Main() != 0) {
            switch ((u32)step_mode) {
            case 0:
                set_XAsector(0x1F000203, 0x3C000214, 0);
                device_cmd_send(768);
                step_mode = step_mode + 1;
                break;
            case 1:
            case 3:
            case 5:
            case 7:
                if (get_CD_State() == 4) {
                    step_mode = step_mode + 1;
                }
                break;
            case 2:
                if (get_CD_State() == 2) {
                    set_XAsector(0x1F000215, 0x3D000221, 0);
                    device_cmd_send(768);
                    step_mode = step_mode + 1;
                }
                break;
            case 4:
                if (get_CD_State() == 2) {
                    set_XAsector(0x160003C6, 0x480003D0, 0);
                    device_cmd_send(768);
                    step_mode = step_mode + 1;
                }
                break;
            case 6:
                if (get_CD_State() == 2) {
                    set_XAsector(0x0B0003AC, 0x380003B7, 0);
                    device_cmd_send(768);
                    step_mode = step_mode + 1;
                }
                break;
            case 8:
                if (get_CD_State() == 2) {
                    Vblnk_Timer_Init();
                    step_mode = step_mode + 1;
                }
                break;
            case 9:
                if ((u32)Vblnk_Timer() >= 181) {
                    step_mode = 0;
                }
                break;
            default:
                game_reset();
                break;
            }
        }
        if ((pad_trig & 0x60) != 0) {
            device_cmd_send(116);
            Fade_Out_Init();
            sub_sub_step_up();
        }
        scroll_poly_disp(0, 15, 0, 240, 640);
        return;

    case 5:
        if (bri_fade_out() != 0) {
            for (i = 0; i < 6; i++) {
                screen_w[i + 1167] = -1;
                screen_w[i + 1199] = -1;
            }
            sub_step_up();
        }
        scroll_poly_disp(0, 15, 0, 240, 640);
        return;

    default:
        game_reset();
        return;
    }
}
