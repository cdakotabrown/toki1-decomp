/* Function: week_day_bustup_init */
/* Address: 0x80074430 */
/* Size: 732 bytes (183 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileEnv: IDO52_PRECOLOR_WEB=a0:53,a1:58,a2:63,a3:185,t4:123 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* `@CompileFlags: -Wc,-pic1`.  The +16 was the retired pic1 compensation; */
/* 5. `@CompileEnv: IDO52_PRECOLOR_WEB=a0:53,a1:58,a2:63,a3:185` names the */
#include "common.h"

extern u8 schedule_cursor;
extern u8 byouki_kind;
extern u16 byouki_state;
extern s32 sched_icon;
extern s32 work_fuku;
extern s32 bustup_clut;
extern volatile s32 bustup_img;
extern volatile s32 bustup_img2;
extern volatile s32 bustup_img3;
extern volatile u8 bustup_sw;
extern u8 now_day;
extern u8 cal_day_kind;
extern u32 screen_w;

extern u8 cal_cw_prio;
extern u8 cal_cw_mode;
extern s32 cal_cw_attr;
extern u8 cal_cw_disp;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s32 cal_cw_hdr;
extern u8 cal_cw_sel;
extern u8 cal_cw_rate;
extern u8 cal_cw_clut;
extern s16 cal_cw_anim;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s16 cw97_x;
extern s16 cw97_y;

extern u8 cw97_prio;
extern u8 cw97_mode;
extern s32 cw97_attr;
extern u8 cw97_disp;
extern s32 cw97_img;
extern s32 cw97_anim_tbl;
extern s32 cw97_hdr;
extern u8 cw97_sel;
extern u8 cw97_rate;
extern u8 cw97_clut;
extern s16 cw97_anim;

extern void class_clear_n(s32 a0);
extern void bustup_addr_set3(s32 a0);
extern void bustup_addr_set2(s32 a0);
extern void bust_up_ptr_set(s32 a0);
extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void sub_step_up(void);

void week_day_bustup_init(void) {
    u8 idx;
    s32 t0;
    u8 *charBase;
    s32 big;
    s32 v18;
    s32 v1c;
    s32 v20;

    class_clear_n(96);
    class_clear_n(97);

    idx = schedule_cursor;
    charBase = (u8 *)&screen_w + (s32)idx * 4;

    t0 = charBase[1694];

    if ((byouki_kind & 0xF) == 3 &&
        (byouki_state & 0x1) == 0 &&
        (sched_icon & 0xF) == 7) {
        bustup_addr_set3(2);
        t0 = 0;
    } else {
        if (charBase[1693] == 4) {
            bustup_addr_set2(work_fuku);
        } else {
            bust_up_ptr_set(work_fuku);
            if (work_fuku == 15 || work_fuku == 16) {
                idx = schedule_cursor;
                charBase = (u8 *)&screen_w + (s32)idx * 4;
                t0 = 0;
                charBase[1694] = 0;
            }
        }
    }

    palette_upload_enqueue(bustup_clut, 16, 1, 1, 0);
    cal_cw_prio = 1;
    cal_cw_mode = 5;
    v18 = bustup_img;
    v1c = bustup_img2;
    v20 = bustup_img3;
    big = 0x01000000;
    cal_cw_attr = big;
    cal_cw_disp = 0x84;
    cal_cw_img = v18;
    cal_cw_anim_tbl = v1c;
    cal_cw_hdr = v20;
    cal_cw_sel = 2;
    if (bustup_sw != 0) {
        cal_cw_rate = 0x82;
    }
    cal_cw_clut = 0x10;
    cal_cw_anim = (s16)t0;
    cal_cw_x = -0x50;
    cal_cw_y = -0x40;

    {
        u8 day, dayField;

        idx = schedule_cursor;
        charBase = (u8 *)&screen_w + (s32)idx * 4;

        if (charBase[1693] == 7) {
            goto done;
        }

        day = now_day;
        dayField = (&cal_day_kind)[day];
        if (dayField == 1) {
            goto done;
        }
        if (dayField == 2) {
            goto done;
        }
        if (dayField == 3) {
            goto done;
        }

        if (charBase[1695] == 15) {
            goto done;
        }
    }

    cw97_prio = 1;
    cw97_mode = 5;
    cw97_attr = big;
    cw97_disp = 0x84;
    cw97_img = v18;
    cw97_anim_tbl = v1c;
    cw97_hdr = v20;
    cw97_sel = 2;
    if (bustup_sw != 0) {
        cw97_rate = 0x82;
    }
    cw97_clut = 0x10;
    cw97_anim = (s16)(t0 + 2);
    cw97_x = -0x18;
    cw97_y = -0x40;

done:
    sub_step_up();
}
