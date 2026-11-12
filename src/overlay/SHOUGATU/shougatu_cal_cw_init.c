/* @PostProc: patch_insn_reg_field 0x24:rt:v1:v0 0x28:rs:v1:v0 0x28:rt:v1:v0 0x2c:rs:v1:v0 0x2c:rt:v0:t6 0x30:rs:v1:v0 0x30:rt:t6:t2 0x34:rd:t8:t7 0x34:rt:v0:t6 0x38:rd:t9:t8 0x38:rt:t8:t7 0x3c:rs:t9:t8 0x3c:rt:t0:t9 0x40:rd:t1:t0 0x40:rt:t0:t9 0x44:rs:t1:t0 0x44:rt:t2:t1 0x48:rs:t6:t2 0x48:rt:t7:t3 0x4c:rd:t3:t4 0x4c:rs:t7:t1 0x4c:rt:t2:t3 0x50:rs:v1:v0 0x50:rt:t3:t4 0x54:rs:v1:v0 0x54:rt:v0:t5 0x5c:rd:t4:t6 0x5c:rt:v0:t5 0x60:rd:t5:t7 0x60:rt:t4:t6 0x64:rs:t5:t7 0x70:rt:t6:t8 0x74:rs:t6:t8 0x74:rt:t6:t8 0x7c:rs:t6:t8 0x7c:rt:t8:t9 0x80:rs:t8:t9 0x80:rt:t9:t0 0x84:rt:t9:t0 0xbc:rt:t0:t2 0xc8:rt:t0:t2 0x108:rt:t7:t3 0x10c:rt:t7:t3 0x114:rt:t2:t4 0x118:rt:t2:t4 0x120:rt:t3:t5 0x124:rt:t4:t6 0x128:rs:t4:t6 0x128:rt:t4:t6 0x12c:rt:t3:t5 0x134:rt:t5:t7 0x138:rs:t5:t7 0x138:rt:t5:t7 0x13c:rt:t4:t6 0x144:rt:t6:t8 0x148:rs:t6:t8 0x148:rt:t6:t8 0x14c:rt:t5:t7 0x154:rt:t8:t9 0x158:rs:t8:t9 0x158:rt:t8:t9 0x15c:rt:t6:t8 0x164:rt:t8:t9 0x17c:rt:t9:t0 0x180:rt:t9:t0 0x188:rt:t0:t2 0x18c:rt:t0:t2 0x1b4:rt:t7:t3 0x1b8:rt:t7:t3 */
/* Address: 0x8013DFE4 */
/* Size: 436 bytes (109 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern void palette_upload_enqueue();
extern void class_clear_n();
extern void event_log_add();
extern s32 strcpy();
extern void shougatu_ptr_set();
extern u8 screen_w[];
extern u8 girl12_flag_hi;
extern s32 g_name;
extern s32 g_name2;
extern s32 ba_string2;
extern s32 shougatu_cal_cw_init_str0;
extern s32 shougatu_cal_cw_init_str1;
extern s32 shougatu_cal_cw_init_str2;
extern s16 shougatu_menu_now_serifu;
extern s32 shougatu_cal_cw_res[];
extern u8 cal_cw_prio;
extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern u8 cal_cw_flag4;
extern u8 cal_cw_sel;
extern u8 cal_cw_bright;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;
extern s16 cal_cw_aux14;
extern s16 cal_cw_anim;
extern s16 cal_cw_frame;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern u8 cal_cw_clut;

void shougatu_cal_cw_init(void)
{
    volatile u8 *p;
    s32 v;

    event_log_add(4, 0);
    p = &screen_w[0];
    v = *(volatile s32 *)(p + 1128);
    p[1129] = (u8)((((((u32)(v << 20) >> 29) + 1) << 1) & 0xE) |
                   (p[1129] & 0xFFF1));
    v = *(volatile s32 *)(p + 1128);
    if (((u32)(v << 20) >> 29) >= 4) {
        girl12_flag_hi = (u8)((girl12_flag_hi & 0xFFF1) | 6);
    }
    strcpy(&g_name, &shougatu_cal_cw_init_str0);
    strcpy(&g_name2, &shougatu_cal_cw_init_str1);
    strcpy(&ba_string2, &shougatu_cal_cw_init_str2);
    shougatu_menu_now_serifu = 9;
    sub_sub_step_up();
    shougatu_ptr_set();
    palette_upload_enqueue(shougatu_cal_cw_res[4], 17, 1, 3, 0);
    class_clear_n(96);
    cal_cw_prio = 12;
    cal_cw_mode = 1;
    cal_cw_attr = 0x41000000;
    cal_cw_disp = 4;
    cal_cw_img = shougatu_cal_cw_res[1];
    cal_cw_anim_tbl = shougatu_cal_cw_res[2];
    cal_cw_hdr = shougatu_cal_cw_res[0];
    cal_cw_aux14 = (s16)(*(s16 *)&shougatu_cal_cw_res[3]);
    cal_cw_anim = 0;
    cal_cw_frame = 0;
    cal_cw_clut = 17;
    cal_cw_x = -160;
    cal_cw_y = -120;
    cal_cw_bright = 0;
    cal_cw_sel = 0;
    cal_cw_flag4 = 8;
}
