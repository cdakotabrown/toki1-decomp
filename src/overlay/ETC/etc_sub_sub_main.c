/* @SymbolStart: 0x8013C328 */
/* @RodataBase: 0x8014CA44 */
/* @SymbolSize: 968 */
/* @PostProc: patch_insn_reg_field 0x70:rt:v0:v1 0x74:rs:v0:v1 0x74:rt:v0:v1 0x7c:rs:v0:v1 0xa8:rs:v0:v1 0xd0:rt:v0:v1 0xd4:rs:v0:v1 0xd4:rt:v0:v1 0xdc:rs:v0:v1 0xe4:rs:v0:v1 0x178:rt:t7:t6 0x184:rt:t6:v1 0x190:rt:t6:v1 0x194:rs:t7:t6 0x194:rt:t7:t6 0x19c:rs:t7:t6 0x19c:rt:t8:t7 0x1a4:rt:t8:t7 0x1b0:rt:v0:v1 0x1b4:rs:v0:v1 0x1b4:rt:v0:v1 0x1bc:rs:v0:v1 0x1c0:rt:t0:t9 0x1c4:rt:t9:t8 0x1d0:rt:t9:t8 0x1e4:rs:t0:t9 0x1e4:rt:t0:t9 0x1ec:rs:t0:t9 0x1ec:rt:t1:t0 0x1f4:rt:t1:t0 0x208:rt:t4:t3 0x220:rt:t2:t1 0x224:rs:t2:t1 0x224:rt:t2:t1 0x22c:rs:t2:t1 0x22c:rt:t3:t2 0x234:rt:t3:t2 0x238:rs:t4:t3 0x238:rt:t4:t3 0x240:rs:t4:t3 0x240:rt:t5:t4 0x248:rt:t5:t4 0x258:rt:t6:t5 0x25c:rs:t6:t5 0x25c:rt:t6:t5 0x264:rs:t6:t5 0x2a0:rt:t7:t6 0x2a4:rs:t7:t6 0x2a4:rt:t7:t6 0x2ac:rs:t7:t6 0x2ac:rt:t8:t7 0x2b0:rt:t8:t7 0x2d0:rt:t9:t8 0x2d4:rs:t9:t8 0x2d8:rt:t0:t9 0x2dc:rs:t0:t9 0x2dc:rt:t0:t9 0x2e4:rs:t0:t9 0x33c:rt:t1:t0 0x340:rs:t1:t0 0x340:rt:t1:t0 0x348:rs:t1:t0 0x348:rt:t2:t1 0x350:rt:t2:t1 */
/* Address: 0x8013C330 */
/* Size: 960 bytes (240 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void indexed_byte_lsb_set(s32, s32);
extern s32  dec_bg_init(s32);
extern void etc_vram_blur(s32, s32);
extern void quad_fade_in_show(void);
extern void etc_frame_save(void);
extern void etc_frame_copy(void);
extern void etc_quad_disp(void);
extern void etc_quad_mode_disp();
extern void etc_sprite_pair_disp(s32);
extern void etc_hizuke_show(void);
extern void etc_event_load(s32);
extern void etc_sub_init(void);
extern void etc_bustup_kuchi_ani(void);
extern void etc_disp_clear(void);
extern void etc_girl_record_search(void);
extern void etc_record_xa_select(void);
extern s32  etc_stage_check(void);
extern void etc_cal_cw_init(void);
extern void etc_model_matrix_set(void);
extern void etc_frame_save2(void);

extern u8  now_girl;
extern s32 pad_trig;
extern s32 step_counter;
extern u8  step_mode;
extern s32 etc_bg_req;
extern s32 etc_record_index;
extern s32 etc_xa_phase;
extern s32 etc_event_step;
extern s32 etc_model_roty;
extern s32 etc_call_choice;

void etc_sub_sub_main(s32 arg) {
    s32 v0;
    s32 v1;

    switch (step_mode) {
    case 0:
        quad_fade_in_show();
        break;
    case 1:
        etc_quad_mode_disp(0, 0);
        etc_event_load(arg);
        v1 = etc_event_step;
        if (v1 >= 3) {
            step_mode = step_mode + 1;
            etc_event_step = 0;
        } else if (v1 == -1) {
            step_mode = step_mode + 1;
        }
        break;
    case 2:
        etc_quad_mode_disp(0, 0);
        v1 = now_girl;
        if (v1 == 12 || v1 == 11) {
            etc_record_xa_select();
        }
        v0 = etc_xa_phase;
        if (v0 != 0 && v0 < 3) {
            break;
        }
        if ((pad_trig & 0x20) == 0) {
            v0 = etc_call_choice;
            if (v0 != 0 && v0 != 1) {
                break;
            }
        }
        step_mode = step_mode + 1;
        etc_xa_phase = 0;
        break;
    case 3:
        etc_quad_mode_disp(step_counter * 2);
        v1 = step_counter;
        if ((u32) v1 < 40) {
            step_counter = v1 + 1;
        } else {
            step_mode = step_mode + 1;
        }
        break;
    case 4:
        etc_frame_save2();
        if (etc_event_step == -1) {
            etc_model_roty = 2048;
            etc_model_matrix_set();
            goto_sub_sub_step(5);
        } else {
            step_mode = step_mode + 1;
        }
        break;
    case 5:
        if (etc_stage_check() == 1) {
            cd_read_start2(14, 0x80197000, 0xA9D0);
            step_mode = step_mode + 1;
        } else {
            step_mode = step_mode + 1;
        }
        break;
    case 6:
        if (etc_stage_check() == 0) {
            if (etc_bg_req == 1) {
                v0 = dec_bg_init(1);
                indexed_byte_lsb_set(1, v0);
                etc_sprite_pair_disp(1);
            }
            etc_sub_init();
            etc_bustup_kuchi_ani();
            etc_hizuke_show();
            step_mode = step_mode + 1;
        }
        if (etc_stage_check() == 1) {
            if (get_CD_State2() & 1) {
                if (etc_bg_req == 1) {
                    v0 = dec_bg_init(1);
                    indexed_byte_lsb_set(1, v0);
                    etc_sprite_pair_disp(1);
                }
                etc_sub_init();
                if (etc_stage_check() == 1) {
                    etc_cal_cw_init();
                }
                etc_bustup_kuchi_ani();
                etc_hizuke_show();
                step_mode = step_mode + 1;
            }
        }
        break;
    case 7:
        etc_frame_save();
        etc_frame_copy();
        etc_disp_clear();
        etc_vram_blur(40, 60);
        v1 = etc_record_index;
        if (v1 < 255) {
            etc_record_index = v1 + 1;
        }
        v0 = etc_call_choice;
        if (v0 == 0 || v0 == 2) {
            etc_girl_record_search();
        }
        sub_sub_step_up();
        break;
    }
    etc_quad_disp();
}
