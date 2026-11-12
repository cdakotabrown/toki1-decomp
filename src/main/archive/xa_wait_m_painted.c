/* Function: xa_wait_m */
/* Address: 0x8005257C */
/* Size: 1260 bytes (315 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_8005257C.py */
#include "types.h"

extern u8 now_girl;
extern u8 k_cnt;
extern u8 k_no;
extern u8 k_mode;
extern u8 voice_level;
extern u8 step_mode;
extern u8 yazirushi_cw_disp;
extern s32 pad_trig;
extern s32 xa_m_tbl;
extern s32 xa_m_tbl2;
extern s32 xa_m_idx;
extern s32 xa_m_mode;

extern s32 get_now_k(void);
extern s32 k_disp_inc(void);
extern void k_disp_switch(s32, s32);
extern s16 *get_k_work(s32);
extern s32 get_CD_State(void);
extern s32 *get_XAsector(void);
extern void set_XAsector(s32, s32, s32);
extern s32 device_cmd_send(s32);
extern void cd_command_issue(s32, s32, s32);
extern void k_disp_goto_line_end(void);
extern void mes_arrow_anim(void);
extern void yazirushi_disp_switch(s32);

void xa_wait_m(void) {
    s32 sp34;
    s32 total_frames;
    s32 total_frames2;
    s32 buttons;
    s32 *colorData;
    s16 *ptr1;
    s16 *ptr2;
    u8 fan_mode;
    u8 cur_frame;
    u8 v0_masked;

    sp34 = 0;
    total_frames = get_now_k();

    fan_mode = k_mode;
    if (fan_mode & 0x80) {
        cur_frame = k_no;
        if (!((u32)(cur_frame + 2) < (u32)total_frames)) {
            sp34 = k_disp_inc();
            total_frames2 = get_now_k();
            if ((k_no + 3) == total_frames2) {
                if (k_cnt >= (u32)(k_no + 3)) {
                    k_disp_switch(total_frames2, 0);
                }
            }
        } else {
            v0_masked = fan_mode & 0x7f;
            if (v0_masked == 0) {
                k_disp_switch(cur_frame, 0);
                ptr1 = get_k_work(k_no + 1);
                ptr2 = get_k_work(k_no + 2);
                ptr1[1] -= 2;
                ptr2[1] -= 2;
                k_mode = k_mode + 1;
            } else {
                ptr1 = get_k_work(cur_frame + 1);
                ptr2 = get_k_work(k_no + 2);
                ptr1[1] -= 2;
                ptr2[1] -= 2;
                if (ptr1[1] < 51) {
                    ptr1[1] = 50;
                    ptr2[1] = 66;
                    k_mode = k_mode - 1;
                    k_no = k_no + 1;
                    k_disp_switch((u8)(k_no) + 2, 1);
                }
            }
        }
    } else {
        v0_masked = voice_level;
        if (v0_masked == 1) {
            if (xa_m_mode != 0) {
                goto do_f04c;
            }
        } else if (v0_masked == 2) {
            if (xa_m_mode != 0) {
                goto do_f04c;
            }
        }
        goto skip_f04c;
do_f04c:
        sp34 = k_disp_inc();
skip_f04c:
        v0_masked = *(volatile u8 *)&voice_level;
        if (v0_masked == 0) {
            sp34 = k_disp_inc();
        }
    }

    total_frames = get_now_k();

    if (xa_m_mode == 0) {
        buttons = get_CD_State();
        if (buttons & 4) {
            xa_m_mode = 1;
        }
    }

    colorData = get_XAsector();

    if (xa_m_mode == 1) {
        buttons = get_CD_State();
        if (buttons & 2) {
            if (xa_m_idx != -1) {
                s32 offset;
                s32 *tbl1;
                s32 *tbl2;
                s32 v1val;
                s32 a0val;

                offset = xa_m_idx << 3;
                tbl1 = (s32 *)(xa_m_tbl2 + offset);
                tbl2 = (s32 *)(xa_m_tbl + offset);
                v1val = tbl2[0];
                a0val = tbl1[0];
                if (v1val != a0val) {
                    if (v1val == colorData[2]) {
                        set_XAsector(a0val, tbl1[1], now_girl);
                        device_cmd_send(768);
                        xa_m_mode = 0;
                    } else {
                        xa_m_mode = 2;
                    }
                } else {
                    xa_m_mode = 2;
                }
            } else {
                xa_m_mode = 2;
            }
        }
    }

    if (sp34 == 1) {
        if (xa_m_mode == 2) {
            if (yazirushi_cw_disp & 0x80) {
                mes_arrow_anim();
            } else {
                yazirushi_disp_switch(1);
            }
        }
    }

    if (sp34 == 1 && voice_level == 0) {
        if (yazirushi_cw_disp & 0x80) {
            mes_arrow_anim();
        } else {
            yazirushi_disp_switch(1);
        }
    }

    if (pad_trig & 0x40) {
        if (sp34 == 1) {
            if (voice_level == 2) {
                xa_m_mode = 2;
                device_cmd_send(116);
                cd_command_issue(9, 0, 0);
            }
            k_mode = 0;
        } else {
            k_disp_goto_line_end();
        }
    }

    if (pad_trig & 0x60) {
        if (sp34 == 1) {
            if (voice_level == 1) {
                if (xa_m_mode == 2) {
                    yazirushi_disp_switch(0);
                    step_mode = step_mode + 1;
                }
                k_mode = 0;
            } else if (voice_level == 2) {
                if (xa_m_mode == 2) {
                    yazirushi_disp_switch(0);
                    step_mode = step_mode + 1;
                }
                k_mode = 0;
            } else {
                yazirushi_disp_switch(0);
                step_mode = step_mode + 1;
                k_mode = 0;
            }
        } else {
            k_disp_goto_line_end();
        }
    }
}
