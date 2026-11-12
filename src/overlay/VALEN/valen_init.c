/* @SymbolStart: 0x80132578 */
/* @SymbolSize: 320 */
/* @PostProc: patch_insn_reg_field 0xa8:rt:t0:at 0xb0:rs:t0:at 0xd4:rt:t1:t0 0xd8:rs:t1:t0 0xd8:rt:t1:t0 0xe0:rt:t2:t1 0xe4:rs:t2:t1 0xe4:rt:t2:t1 0xe8:rt:t1:t0 0xf0:rt:t3:t2 0xf4:rs:t3:t2 0xf4:rt:t3:t2 0xf8:rt:t2:t1 0x100:rt:t3:t2 0x118:rt:t4:t3 0x11c:rt:t4:t3 0x128:rt:t5:t4 0x12c:rs:t5:t4 0x12c:rt:t5:t4 0x134:rs:t5:t4 0x134:rt:t6:t5 0x13c:rt:t6:t5 */
/* valen_init - VALEN overlay, Address: 0x80132580, Size: 312 bytes (78 insns) */
#include "types.h"

extern u8 day_flag;
extern s32 ba_string;
extern s32 valen_init_str0;
extern u8 vblank_flag;
extern s32 vb_ready;
extern u8 screen_bri;
extern u8 fade_bri;

extern s32 valen_ss_s;
extern s32 valen_ss_s_kind;
extern s32 valen_sss_so;
extern s16 valen_now_s;
extern s16 valen_now_s_in;
extern u8 now_girl;
extern s32 valen_msg_res_tbl[][13];

extern void draw2d3d(s32, s32);
extern void class_switch(s32);
extern void prim_init(void);
extern void class_clear_all(s32);
extern void back_clear_switch(s32);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void strcpy(s32 *, s32 *);
extern void shared_system_state_reset(void);
extern void bustup_ptr_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void yazirushi_off(void);
extern void valen_res_init(void);
extern void date_name_set(s32);
extern void sub_sub_step_up(void);

void valen_init(void)
{
    day_flag = (u8)(day_flag | 0x10);
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    strcpy(&ba_string, &valen_init_str0);
    vblank_flag = 0;
    vb_ready = 1;
    shared_system_state_reset();
    screen_bri = 128;
    fade_bri = 0;
    *(u8 *)0x800BBE44 = 0;
    bustup_ptr_init();
    hizuke_init();
    message_window_init();
    yazirushi_off();
    valen_res_init();
    valen_ss_s = valen_msg_res_tbl[0][4];
    valen_ss_s_kind = valen_msg_res_tbl[1][4];
    valen_sss_so = valen_msg_res_tbl[2][4];
    valen_now_s = 0;
    valen_now_s_in = 0;
    now_girl = 12;
    date_name_set(12);
    day_flag = (u8)(day_flag | 0x08);
    sub_sub_step_up();
}
