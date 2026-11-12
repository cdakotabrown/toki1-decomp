/* @CompileEnv: IDO52_FORCE_WEB_COLOR=4:v1 */
/* @SymbolStart: 0x80132834 */
/* @RodataBase: 0x80139458 */
/* @SymbolSize: 852 */
/* Address: 0x8013283C */
/* Size: 844 bytes (211 insns) */
#include "types.h"

extern u8 now_sub_sub_step;

extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern void srn_vram_set(s32, s32, s32, s32);
extern void srn_init(s32, s32, s32);
extern void mod_init(s32, s32, s32);
extern void device_cmd_send(s32);
extern s32  get_CD_State(void);
extern void set_XAsector(s32, s32, s32);
extern void game_reset(void);
extern void Vblnk_Timer_Init(void);
extern s32  Vblnk_Timer(void);
extern void scroll_poly_disp(s32, s32, s32, s32, s32);
extern s32  bri_fade_out(void);
extern void fade_timer_init(void);
extern s32  Fade_In_Main(void);
extern void Fade_Out_Init(void);

extern s32 screen_w[];
extern u8  screen_bri;
extern s32 pad_trig;
extern u8  step_mode;
extern s32 en_nichi_intro_mod_hdr;
extern s32 en_nichi_intro_srn_ptr;
extern s32 en_nichi_intro_mod_src;

void en_nichi_intro_main(void) {
    s32 i;

    switch (now_sub_sub_step) {
    case 0:
        screen_bri = 0;
        mod_init(0, en_nichi_intro_mod_src, en_nichi_intro_mod_hdr);
        srn_init(0, en_nichi_intro_srn_ptr, 0);
        srn_vram_set(0, 5, 1408, 240);
        fade_timer_init();
        sub_sub_step_up();
        break;
    case 1:
        if (get_CD_State() == 2) {
            sub_sub_step_up();
        }
        break;
    case 2:
        if (Fade_In_Main() != 0) {
            switch (step_mode) {
            case 0:
                set_XAsector(0x1F0001D2, 0x390001E3, 0);
                device_cmd_send(768);
                step_mode = step_mode + 1;
                break;
            case 1:
            case 3:
            case 5:
                if (get_CD_State() == 4) {
                    step_mode = step_mode + 1;
                }
                break;
            case 2:
                if (get_CD_State() == 2) {
                    set_XAsector(0x1F0001E4, 0x3A0001F2, 0);
                    device_cmd_send(768);
                    step_mode = step_mode + 1;
                }
                break;
            case 4:
                if (get_CD_State() == 2) {
                    set_XAsector(0x1F0001F3, 0x3B000202, 0);
                    device_cmd_send(768);
                    step_mode = step_mode + 1;
                }
                break;
            case 6:
                if (get_CD_State() == 2) {
                    Vblnk_Timer_Init();
                    step_mode = step_mode + 1;
                }
                break;
            case 7:
                if ((u32) Vblnk_Timer() >= 181) {
                    step_mode = 0;
                }
                break;
            default:
                game_reset();
                break;
            }
        }
        if (pad_trig & 0x60) {
            device_cmd_send(116);
            Fade_Out_Init();
            sub_sub_step_up();
        }
        scroll_poly_disp(0, 15, 0, 240, 640);
        break;
    case 3:
        if (bri_fade_out() != 0) {
            for (i = 0; i < 6; i++) {
                screen_w[i + 1167] = -1;
                screen_w[i + 1199] = -1;
            }
            device_cmd_send(513);
            sub_step_up();
        }
        scroll_poly_disp(0, 15, 0, 240, 640);
        break;
    default:
        game_reset();
        break;
    }
}
