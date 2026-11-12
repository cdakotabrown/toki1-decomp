/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: swap_instructions --offset1 0x8C --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x80 */
/* @PostProc: remap_stack_slots --map '108:112,124:128,126:130,132:136,134:138,136:140,140:144,154:158,156:160,170:174' */
/* @PostProc: patch_insn_reg_field 0xf8:rt:t7:t8 0xfc:rs:t7:t8 0xfc:rt:v1:t8 0x104:rs:v1:t8 0x104:rt:t8:t9 0x110:rs:t8:t9 0x11c:rt:t9:t0 0x120:rs:t9:t0 0x120:rt:v0:t0 0x128:rs:v0:t0 0x128:rt:t0:t1 0x12c:rs:t0:t1 0x138:rt:t1:t2 0x13c:rt:t2:t3 0x140:rt:t2:t3 0x144:rt:t1:t2 */
/* olh_minigame_page_init - OLH overlay, Address: 0x80134FC0, Size: 388 bytes (97 insns) */
#include "types.h"

extern void k_sub_reset(void);
extern s32 set_kanji_string(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void menu_set(s32 a0, s32 a1, s16 *a2, s16 *a3, s16 *a4, s16 *a5);
extern void k_disp_start(s32 a0);
extern void sub_sub_step_up(void);

extern u8  olh_main60_0_str0[];
extern s32 olh_page60_tbl[];

void olh_minigame_page_init(void) {
    s32 s3_y;
    s32 s0_ret;
    s32 fp_h;
    s32 s7_w;
    s32 s6_x;
    s16 x_buf[8];
    s16 y_buf[8];
    s16 w_buf[8];
    s16 h_buf[8];
    s32 s0;
    s16 *s1_x;
    s16 *s2_y;
    s16 *s4_w;
    s16 *s5_h;
    u8 t8;
    u32 t0;
    s32 pad;

    (void)&pad;
    k_sub_reset();

    set_kanji_string(-80, -64, 15, (s32)olh_main60_0_str0, 0);

    s0    = 0;
    s1_x  = x_buf;
    s2_y  = y_buf;
    s3_y  = -50;
    s4_w  = w_buf;
    s5_h  = h_buf;
    fp_h  = 16;
    s7_w  = 192;
    s6_x  = -80;

    do {
        *s1_x = (s16)s6_x;
        *s2_y = (s16)s3_y;
        *s4_w = (s16)s7_w;
        *s5_h = (s16)fp_h;
        if (s0 < 4) {
            s0_ret = set_kanji_string(*s1_x, *s2_y, 15,
                olh_page60_tbl[s0], 0);
        }
        s0++;
        s1_x++;
        s2_y++;
        s3_y += 16;
        s4_w++;
        s5_h++;
    } while (s0 != 8);

    s0_ret = set_kanji_string(x_buf[7], y_buf[7], 15, olh_page60_tbl[7], 0);

    t8 = *(u8 *)(0x800F0000U + 559);
    w_buf[5] = 0;
    w_buf[4] = 0;
    w_buf[6] = 0;
    if (!(t8 & 1)) {
        w_buf[0] = 0;
    }
    t0 = *(u32 *)(0x800F0000U + 560);
    if (!(t0 & 1)) {
        w_buf[1] = 0;
    }

    menu_set(0, 8, x_buf, y_buf, w_buf, h_buf);
    k_disp_start(s0_ret);
    sub_sub_step_up();
}
