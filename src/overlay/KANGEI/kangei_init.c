/* @SymbolSize: 464 */
/* @PostProc: swap_instructions --offset1 0xD0 --offset2 0xD4 */
/* @PostProc: patch_insn_reg_field 0xc4:rt:a0:v1 0xd0:rs:a0:v1 0xd0:rt:a0:v1 0xd8:rt:v1:a0 0xdc:rt:v1:a0 0xe4:rs:t9:v1 0xe4:rt:a0:t9 0x124:rt:t2:t3 0x158:rt:v0:t1 0x15c:rs:v0:t1 0x15c:rt:v0:t1 0x160:rt:t1:t2 0x164:rs:v0:t1 0x170:rt:t1:t2 0x174:rs:t2:t3 0x18c:rt:t3:t4 0x190:rs:t3:t4 0x190:rt:t3:t4 0x198:rt:t4:t5 0x19c:rs:t4:t5 0x19c:rt:t4:t5 0x1a0:rt:t3:t4 0x1a8:rt:t5:t6 0x1ac:rs:t5:t6 0x1ac:rt:t5:t6 0x1b0:rt:t4:t5 0x1b8:rt:t5:t6 */
/* kangei_init - KANGEI overlay, Address: 0x80133D38, Size: 464 bytes (116 insns). */
#include "types.h"

extern void draw2d3d(s32, s32);
extern void class_switch(s32);
extern void prim_init(void);
extern void back_clear_switch(s32);
extern void class_clear_all(s32);
extern void message_window_init(void);
extern void hizuke_init(void);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void shared_system_state_reset(void);
extern void parameter_disp_switch(s32);
extern void icon_disp_switch(s32);
extern void bustup_ptr_init(void);
extern void yazirushi_off(void);
extern void date_name_set(s32);
extern void kangei_ptr_tbl2_init(void);
extern void sub_sub_step_up(void);

extern volatile u8 vblank_flag;
extern volatile s32 vb_ready;
extern volatile u8 fade_bri;
extern volatile s16 kangei_now_s;
extern volatile s16 kangei_now_s_in;
extern volatile u8 now_girl;
extern volatile u16 basyo_pack;
extern u8 screen_w[2048];
extern s16 girl10_tokimeki;
extern s16 girl10_yuukou;
extern s16 girl0_yuukou;
extern s16 girl0_tokimeki;
extern volatile u8 now_year;
extern volatile u8 girl_no;
extern s32 kangei_ss_s;
extern s32 kangei_ss_s_kind;
extern s32 kangei_sss_so;
extern volatile u8 kangei_serifu_mode;
extern volatile u8 kangei_icon_flag;
extern s32 kangei_msg_res_tbl[][8];

void
kangei_init(void)
{
    u32 spriteIdx;
    s32 i;
    s32 offs;
    u8 lookup;
    s32 sum1, sum2;
    u8 charIdx;
    u8 *base;
    u8 *p;

    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    back_clear_switch(1);
    class_clear_all(0);
    message_window_init();
    hizuke_init();
    tpage_buf_clear();
    text_sys_init();

    vblank_flag = 0;
    vb_ready = 1;
    fade_bri = 0;

    shared_system_state_reset();
    parameter_disp_switch(0);
    icon_disp_switch(0);

    kangei_now_s = 0;
    kangei_now_s_in = 0;

    bustup_ptr_init();
    yazirushi_off();

    now_girl = 255;

    spriteIdx = (u32)basyo_pack >> 12;

    base = screen_w;
    i = 0;
    do {
        offs = i * 56;
        if (spriteIdx == ((u32)base[offs + 458] >> 4)) {
            now_girl = i;
            break;
        }
        i = i + 1;
        i = (s16)i;
    } while (i < 11);

    p = (u8 *)0x800F0000;
    if (spriteIdx == 10) {
        sum1 = girl10_yuukou + girl10_tokimeki;
        sum2 = girl0_tokimeki + girl0_yuukou;
        if (sum2 < sum1) {
            lookup = now_year;
            if ((u32)lookup >= 96U) {
                now_girl = 10;
            }
        }
    }

    charIdx = p[-2369];
    girl_no = charIdx;

    date_name_set(charIdx);
    kangei_ptr_tbl2_init();

    kangei_ss_s = kangei_msg_res_tbl[0][6];
    kangei_ss_s_kind = kangei_msg_res_tbl[1][6];
    kangei_sss_so = kangei_msg_res_tbl[2][6];

    kangei_serifu_mode = 0;
    kangei_icon_flag = 0;

    sub_sub_step_up();
}
