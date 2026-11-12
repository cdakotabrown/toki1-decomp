/* Address: 0x80052600 */
/* Size: 1152 bytes (288 instructions) */
/* Region: Game */
/* @Measured: 2026-09-24 11/188 exact (5.9%), Body 185 vs target 288 words, Fixup 2 -- SEMI (stub-grade body) */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 xa_m_mode;
extern s32 xa_m_idx;
extern s32 xa_m_tbl2;
extern s32 xa_m_tbl;
extern u8 now_girl;
extern u8 yazirushi_cw_disp;
extern u8 voice_level;
extern s32 pad_trig;
extern u8 k_mode;
extern u8 step_mode;

extern s32 get_CD_State(void);
extern void *get_XAsector(void);
extern void set_XAsector(s32 a, s32 b, u8 c, void *d);
extern void device_cmd_send(s32 a);
extern void cd_command_issue(s32 a, s32 b, s32 c);
extern void k_disp_goto_line_end(void);
extern void yazirushi_disp_switch(s32 a);
extern void mes_arrow_anim(void);

void func_80052600(s32 arg5, void *argA, s32 argD) {
    s32 temp_a0;
    s32 temp_t3;
    s32 temp_t4;
    s32 temp_v1;
    u8 *temp_a3;
    u8 *temp_v0;

    (void)arg5;

    if ((xa_m_mode == 0) && (get_CD_State() & 4)) {
        xa_m_mode = 1;
    }
    temp_v0 = (u8 *)get_XAsector();
    if (xa_m_mode == 1) {
        argA = temp_v0;
        if (get_CD_State() & 2) {
            temp_t3 = xa_m_idx;
            if (temp_t3 != -1) {
                temp_t4 = temp_t3 * 8;
                temp_a3 = (u8 *)(xa_m_tbl2 + temp_t4);
                temp_v1 = *(s32 *)(xa_m_tbl + temp_t4);
                temp_a0 = *(s32 *)(temp_a3 + 0);
                if ((temp_v1 != temp_a0) && (temp_v1 == *(s32 *)((u8 *)argA + 8))) {
                    set_XAsector(temp_a0, *(s32 *)(temp_a3 + 4), now_girl, temp_a3);
                    device_cmd_send(0x300);
                    xa_m_mode = 0;
                } else {
                    xa_m_mode = 2;
                }
            } else {
                xa_m_mode = 2;
            }
        }
    }
    if ((argD == 1) && (xa_m_mode == 2)) {
        if (yazirushi_cw_disp & 0x80) {
            mes_arrow_anim();
        } else {
            yazirushi_disp_switch(1);
        }
    }
    if ((argD == 1) && (voice_level == 0)) {
        if (yazirushi_cw_disp & 0x80) {
            mes_arrow_anim();
        } else {
            yazirushi_disp_switch(1);
        }
    }
    if (pad_trig & 0x40) {
        if (argD == 1) {
            if (voice_level == 2) {
                xa_m_mode = 2;
                device_cmd_send(0x74);
                cd_command_issue(9, 0, 0);
            }
            k_mode = 0;
        } else {
            k_disp_goto_line_end();
        }
    }
    if (pad_trig & 0x60) {
        if (argD == 1) {
            if (voice_level == 1) {
                if (xa_m_mode == 2) {
                    yazirushi_disp_switch(0);
                    step_mode = (u8)(step_mode + 1);
                }
                k_mode = 0;
                return;
            }
            if (voice_level == 2) {
                if (xa_m_mode == 2) {
                    yazirushi_disp_switch(0);
                    step_mode = (u8)(step_mode + 1);
                }
                k_mode = 0;
                return;
            }
            yazirushi_disp_switch(0);
            step_mode = (u8)(step_mode + 1);
            k_mode = 0;
            return;
        }
        k_disp_goto_line_end();
    }
}
