/* Function: vacation_day_bustup_init */
/* Address: 0x80075110 */
/* Size: 704 bytes (176 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: remap_stack_slots --map 32:48 */
/* @PostProc: patch_insn_reg_field --patch 0x174:rt:v0:a0 --patch 0x178:rt:v0:a0 --patch 0x178:rs:v0:a0 --patch 0x184:rs:v0:a0 --patch 0x190:rs:v0:a0 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 schedule_cursor;
extern u8 byouki_kind;
extern u16 byouki_state;
extern s32 sched_icon;
extern u8 bustup_sw;
extern u8 schedule_buf;
extern u8 screen_w[];

extern u8 cal_cw_prio;
extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern u8 cal_cw_sel;
extern u8 cal_cw_rate;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s16 cal_cw_anim;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;
extern u8 cal_cw_clut;

extern u8 cw97_prio;
extern u8 cw97_mode;
extern u8 cw97_disp;
extern u8 cw97_sel;
extern u8 cw97_rate;
extern s32 cw97_img;
extern s32 cw97_anim_tbl;
extern s16 cw97_anim;
extern s16 cw97_x;
extern s16 cw97_y;
extern s32 cw97_hdr;
extern s32 cw97_attr;
extern u8 cw97_clut;

extern s32 work_fuku;
extern s32 bustup_clut;
extern s32 bustup_img;
extern s32 bustup_img2;
extern s32 bustup_img3;

extern void class_clear_n(s32);
extern void bustup_addr_set3(s32);
extern void bustup_addr_set2(s32);
extern void bustup_addr_set(s32);
extern s32 get_param_top_girl(s32);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void sub_step_up(void);
extern void goto_sub_step(s32);

void vacation_day_bustup_init(void) {
    u8 *base;
    s32 v1;

    class_clear_n(96);

    base = &screen_w[schedule_cursor * 4];
    v1 = base[1694];

    if ((byouki_kind & 0xF) == 3 && !(byouki_state & 1) && (sched_icon & 0xF) == 7) {
        bustup_addr_set3(2);
        v1 = 0;
    } else if (base[1693] == 4) {
        bustup_addr_set2(work_fuku);
        if (get_param_top_girl(4) == 15) {
            goto common_path;
        }
        cw97_prio = 1;
        cw97_mode = 5;
        cw97_attr = 0x01000000;
        cw97_disp = 132;
        cw97_img = bustup_img;
        cw97_anim_tbl = bustup_img2;
        cw97_hdr = bustup_img3;
        cw97_sel = 2;
        if (bustup_sw != 0) {
            cw97_rate = 130;
        }
        cw97_clut = 16;
        cw97_anim = (s16)(v1 + 2);
        cw97_x = -24;
        cw97_y = -64;
    } else {
        bustup_addr_set(work_fuku);
        if (work_fuku == 6 || work_fuku == 7) {
            v1 = 0;
            screen_w[schedule_cursor * 4 + 1694] = 0;
        }
    }

common_path:
    palette_upload_enqueue(bustup_clut, 16, 1, 1, 0);

    cal_cw_prio = 1;
    cal_cw_mode = 5;
    cal_cw_attr = 0x01000000;
    cal_cw_disp = 132;
    cal_cw_img = bustup_img;
    cal_cw_anim_tbl = bustup_img2;
    cal_cw_hdr = bustup_img3;
    cal_cw_sel = 2;
    if (bustup_sw != 0) {
        cal_cw_rate = 130;
    }
    cal_cw_clut = 16;
    cal_cw_anim = (s16)v1;
    cal_cw_x = -80;
    cal_cw_y = -64;

    if (schedule_buf == 66) {
        sub_step_up();
    } else {
        goto_sub_step(1);
    }
}
