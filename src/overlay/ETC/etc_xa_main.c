/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=occ:1 IDO52_FORCE_WEB_COLOR=20:v1,23:v1 IDO52_FORCE_WEB_RANGE_SPLIT=18 */
/* @SymbolStart: 0x8013CF1C */
/* @SymbolSize: 824 */
/* @RodataBase: 0x8014CA78 */
/* Address: 0x8013CF24 (true entry 0x8013CF1C, cross-prologue head absorbed) */
/* Size: 824 bytes (206 instructions incl. the 2-word head) */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern void goto_sub_step(s32);
extern void device_cmd_send(s32);
extern s32  get_CD_State(void);
extern void set_XAsector(s32, s32, s32);
extern void etc_xa_fade_disp(s32);
extern void etc_xa_step(s32);
extern void etc_model_matrix_set(void);

extern u8  now_girl;
struct tmr { s32 timer; s32 pad[2]; u8 step; };
extern struct tmr step_state;
extern s32 Bg;
extern s32 etc_xa_pair_tbl[];
extern s32 etc_record_index;
extern s32 etc_xa_phase;
extern s32 etc_event_step;
extern s32 etc_model_roty;
extern s32 etc_call_choice;
extern s32 etc_call_pass_cnt;
extern s32 etc_skip_req;

void etc_xa_main(void)
{
    s32 v1;
    s32 i;

    if (etc_record_index == 0 || etc_skip_req == 1) {
        step_state.step = 5;
    }
    switch (step_state.step) {
    case 0:
        if ((u32) step_state.timer < 121) {
            return;
        }
        v1 = now_girl;
        if (v1 == 0) {
            etc_xa_step(20);
            if (etc_xa_phase < 3) {
                return;
            }
            step_state.step = step_state.step + 1;
            etc_xa_phase = 0;
        } else if (v1 == 2) {
            etc_xa_step(8);
            v1 = etc_xa_phase;
            if (v1 == 2) {
                step_state.timer = 0;
            }
            if (v1 < 3) {
                return;
            }
            if ((u32) step_state.timer >= 61) {
                step_state.step = step_state.step + 1;
                etc_xa_phase = 0;
            } else {
                step_state.timer = step_state.timer + 1;
            }
        } else if (v1 == 4) {
            etc_xa_step(13);
            if (etc_xa_phase < 3) {
                return;
            }
            step_state.step = step_state.step + 1;
            etc_xa_phase = 0;
        } else if (v1 == 12) {
            etc_xa_step(7);
            if (etc_xa_phase < 3) {
                return;
            }
            step_state.step = step_state.step + 1;
            etc_xa_phase = 0;
        } else {
            step_state.step = step_state.step + 1;
        }
        break;
    case 1:
        set_XAsector(etc_xa_pair_tbl[now_girl * 2], etc_xa_pair_tbl[now_girl * 2 + 1], now_girl);
        device_cmd_send(768);
        step_state.step = step_state.step + 1;
        break;
    case 2:
        if ((get_CD_State() & 4) != 0) {
            step_state.step = step_state.step + 1;
        }
        break;
    case 3:
        if ((get_CD_State() & 2) != 0) {
            step_state.timer = 0;
            step_state.step = step_state.step + 1;
        }
        break;
    case 4:
        if ((u32) step_state.timer < 121) {
            break;
        }
        step_state.timer = 0;
        step_state.step = step_state.step + 1;
        break;
    case 5:
        etc_xa_fade_disp(step_state.timer);
        if ((u32) step_state.timer < 301) {
            break;
        }
        etc_skip_req = 0;
        etc_event_step = 0;
        etc_record_index = 0;
        etc_call_choice = 0;
        etc_call_pass_cnt = 0;
        Bg = 0x80000000;
        (*(s32 *)&((LIBGS_BG *)&Bg)[1].attribute) = 0x80000000;
        etc_model_roty = 3072;
        etc_model_matrix_set();
        goto_sub_step(255);
        break;
    }
}
