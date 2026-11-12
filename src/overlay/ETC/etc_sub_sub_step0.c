/* @RodataBase: 0x8014CA30 */
/* @SymbolStart: 0x8013BFFC */
/* @SymbolSize: 812 */
/* @CompileFlags: -cckr */
#include "types.h"

extern u8 step_mode;
extern s32 Bg;
extern s32 etc_bg_req;
extern s32 etc_record_index;
extern s32 etc_event_phase;
extern s32 etc_event_step;
extern s32 etc_model_roty;
extern s32 etc_call_choice;

void etc_sub_sub_step0()
{
    switch (step_mode) {
    case 0:
        etc_frame_save2();
        Bg = 0x80000000;
        if (etc_event_phase > 0) {
            etc_event_step = 0;
        }
        if (etc_event_step == -1) {
            etc_model_roty = 2048;
            etc_model_matrix_set();
            sub_step_up();
        } else {
            step_mode++;
        }
        break;
    case 1:
        if (etc_event_phase > 0) {
            etc_event_load(1);
            if (etc_event_step >= 3) {
                if (etc_event_phase == 1) {
                    etc_event_step = 0;
                } else {
                    etc_event_phase = 0;
                    etc_event_step = 0;
                }
                step_mode++;
            } else if (etc_event_step == -1) {
                etc_model_roty = 2048;
                etc_model_matrix_set();
                goto_sub_sub_step(5);
            }
        } else {
            step_mode++;
        }
        break;
    case 2:
        if (etc_stage_check() == 1) {
            cd_read_start2(14, 0x80197000, 0xA9D0);
            step_mode++;
        } else {
            step_mode++;
        }
        break;
    case 3:
        if (etc_stage_check() == 0) {
            if (etc_bg_req == 1) {
                indexed_byte_lsb_set(1, dec_bg_init(1));
                etc_sprite_pair_disp(1);
            }
            etc_sub_init();
            etc_bustup_kuchi_ani();
            etc_hizuke_show();
            step_mode++;
        } else if (etc_stage_check() == 1 && (get_CD_State2() & 1)) {
            if (etc_bg_req == 1) {
                indexed_byte_lsb_set(1, dec_bg_init(1));
                etc_sprite_pair_disp(1);
            }
            etc_sub_init();
            if (etc_stage_check() == 1) {
                etc_cal_cw_init();
            }
            etc_bustup_kuchi_ani();
            etc_hizuke_show();
            step_mode++;
        }
        break;
    case 4:
        etc_frame_save();
        etc_frame_copy();
        etc_disp_clear();
        etc_vram_blur(40, 60);
        if (etc_record_index < 255) {
            etc_record_index++;
        }
        if (etc_call_choice == 0 || etc_call_choice == 2) {
            etc_girl_record_search();
        }
        sub_sub_step_up();
        break;
    }
    etc_quad_disp();
}
