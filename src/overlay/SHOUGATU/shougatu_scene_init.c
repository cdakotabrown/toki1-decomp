/* @PostProc: patch_insn_reg_field 0x74:rt:v0:t6 0x78:rs:v0:t6 0x78:rt:v0:t6 0x84:rd:t6:t7 0x84:rt:v0:t6 0x88:rd:t7:t8 0x88:rt:t6:t7 0x8c:rs:t7:t8 0x8c:rt:v1:t9 0x94:rt:t8:t1 0x98:rs:t8:t1 0x98:rt:t8:t1 0x9c:rt:v1:t9 0xa4:rt:t8:t1 0xac:rd:t9:t2 0xac:rt:t6:t7 0xb0:rt:v1:t9 0xb4:rs:t9:t2 0xe4:rt:t1:t3 0xe8:rt:t2:t4 0xec:rd:t2:t4 0xec:rs:t2:t4 0xf0:rd:t1:t3 0xf0:rs:t1:t3 0xf8:rs:t1:t3 0xf8:rt:t1:t3 0xfc:rs:t2:t4 0xfc:rt:t2:t4 0x108:rt:t1:t3 0x110:rt:t2:t4 0x130:rt:t3:t6 0x134:rs:t3:t6 0x134:rt:t3:t6 0x138:rt:v1:v0 0x13c:rs:v1:v0 0x13c:rt:v1:v0 0x140:rd:t4:t8 0x140:rt:t3:t6 0x144:rd:t5:t1 0x144:rs:v1:v0 0x144:rt:t4:t8 0x148:rs:t5:t1 0x148:rt:v0:t9 0x14c:rt:t6:t5 0x150:rt:v0:t9 0x154:rt:v0:t9 0x15c:rd:t8:t2 0x15c:rs:v1:v0 0x164:rs:t8:t2 0x164:rt:t6:t5 */
/* Address: 0x8013765C */
/* Size: 360 bytes (90 instructions) */
#include "types.h"

extern void prim_init(void);
extern void sub_step_up(void);
extern void draw2d3d(s32, s32);
extern void back_clear_switch(s32);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);
extern void tpage_buf_clear(void);
extern void class_switch(s32);
extern void class_clear_all(s32);
extern void text_sys_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void shared_system_state_reset(void);

extern s32 girl_head_tbl[];
extern s32 girl_body_tbl[];
extern s32 girl_seq_tbl[];
extern u8 cdq_footer_chk;
extern u8 schedule_cursor;
extern u8 now_girl;
extern volatile u8 screen_w[];
extern s32 work_tarao;
extern u8 shougatu_date_girl;
extern u8 shougatu_date_kind;

void shougatu_scene_init(void)
{
    s32 sel;
    s32 p1;
    s32 p2;
    s32 idx;

    cdq_footer_chk = 0;
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    hizuke_init();
    message_window_init();
    shared_system_state_reset();

    sel = *(volatile u8 *)((s32)screen_w + schedule_cursor * 4 + 1693);
    now_girl = (u8)sel;
    shougatu_date_kind = (u8)work_tarao;
    shougatu_date_girl = (u8)sel;
    sel = screen_w[schedule_cursor * 4 + 1693];
    if (sel == 2 || sel == 6) {
        p1 = 0xC071;
        p2 = 0xC053;
    } else {
        p1 = 0xC051;
        p2 = 0xC02B;
    }
    device_load_start(1, p1, p2, girl_head_tbl[sel], girl_body_tbl[sel],
                  girl_seq_tbl[sel]);
    if (device_load_poll() == 1) {
        sub_step_up();
    }
    idx = screen_w[schedule_cursor * 4 + 1693];
    *(s16 *)&screen_w[idx * 56 + 450] = 70;
    sub_step_up();
}
