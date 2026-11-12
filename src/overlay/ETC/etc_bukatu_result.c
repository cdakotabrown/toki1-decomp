/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic0 */
/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=1 IDO52_CONST_INREG_VALUES=255 IDO52_NO_LOOP_TEST_REPLACEMENT=bb:1 IDO52_AS1_NO_LA_PULL=3 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x150 --offset2 0x154 */
/* @PostProc: patch_insn_reg_field 0x78:rt:t6:v0 0x7c:rs:t6:v0 0x7c:rt:t6:v0 0x84:rs:t6:v0 0xd8:rt:t7:t6 0xdc:rs:t7:t6 0xdc:rt:t7:t6 0xe4:rs:t7:t6 0xe4:rt:t8:t7 0xe8:rs:t8:t7 0x104:rt:t9:t8 0x108:rs:t9:t8 0x108:rt:t9:t8 0x110:rs:t9:t8 0x110:rt:t0:t9 0x118:rt:t0:t9 0x11c:rt:t1:t0 0x120:rs:t1:t0 0x120:rt:t1:t0 0x128:rs:t1:t0 0x12c:rt:t2:t1 0x130:rs:t2:t1 0x130:rt:t2:t1 0x138:rs:t2:t1 0x13c:rt:t3:t2 0x140:rs:t3:t2 0x140:rt:t3:t2 0x148:rs:t3:t2 0x170:rt:t4:t3 0x178:rs:t4:t3 0x1c0:rt:v1:v0 0x1c8:rs:v1:v0 0x1c8:rt:t5:t4 0x1cc:rt:t5:t4 0x1ec:rt:t6:t5 0x1f0:rs:t6:t5 0x1f0:rt:t6:t5 0x1f8:rs:t6:t5 0x1f8:rt:t7:t6 0x200:rt:t7:t6 0x204:rd:t8:t7 0x204:rt:v1:v0 0x20c:rt:t8:t7 0x21c:rt:t9:t8 0x220:rs:t9:t8 0x220:rt:t9:t8 0x228:rs:t9:t8 0x228:rt:t0:t9 0x230:rt:t0:t9 0x234:rt:t1:v0 0x238:rs:t1:v0 0x238:rt:t1:v0 0x240:rd:t2:t0 0x240:rt:t1:v0 0x244:rt:t2:t0 0x258:rt:t3:t1 0x25c:rs:t3:t1 0x25c:rt:t3:t1 0x264:rs:t3:t1 0x264:rt:t4:t2 0x268:rt:t4:t2 0x288:rt:t5:t3 0x28c:rs:t5:t3 0x29c:rt:t6:t4 0x2a0:rt:t7:t5 0x2a4:rt:t8:t6 0x2a8:rt:t9:t7 0x2ac:rt:t0:t8 0x2b0:rt:t1:t9 0x2b4:rt:t1:t9 0x2b8:rt:t0:t8 0x2bc:rt:t9:t7 0x2c0:rt:t8:t6 0x2c4:rt:t7:t5 0x2c8:rt:t6:t4 0x318:rt:t2:t0 0x31c:rs:t2:t0 0x324:rt:t3:t1 0x328:rt:t4:t2 0x32c:rt:t5:t3 0x330:rt:t6:t4 0x334:rt:t7:t5 0x338:rt:t8:t6 0x33c:rt:t8:t6 0x340:rt:t7:t5 0x344:rt:t6:t4 0x348:rt:t5:t3 0x34c:rt:t4:t2 0x350:rt:t3:t1 */
/* Address: 0x80145BA0 */
/* Size: 968 bytes (242 instructions) */
/* `@PostProc: pad_trailing_nop --count 240` -- i.e. the entire body was */
#include "types.h"

extern void prim_init(void);
extern void sub_step_up(void);
extern void goto_step(s32);
extern void goto_sub_sub_step(s32);
extern void draw2d3d(s32, s32);
extern void back_clear_switch(s32);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void device_cmd_send(s32);
extern s32 get_CD_State2(void);
extern void set_XAsector(s32, s32, s32);
extern void cd_read_start2(s32, s32, s32);
extern void class_clear_f(void);
extern void bg_set();
extern void white_light(s32);
extern void disp_mode_set(s32);
extern void printf();

extern u8 screen_w[];
extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern s32 pad_data;
extern u8 now_girl;
extern u8 step_mode;
extern u8 album_work[];
extern s32 etc_bukatu_lba_tbl[];
extern u8 etc_bukatu_clear_fmt[];
extern u8 etc_bukatu_nonclear_fmt[];
extern u8 etc_bukatu_result_str0[];

void etc_bukatu_result(void)
{
    u8 *buf[11];
    s32 *dst;
    s32 val;
    u8 *p;
    u8 *q;
    s32 i;
    s32 cnt;
    s32 last;
    s32 v;
    u8 c;

    dst = (s32 *)buf;
    val = 0x80180000;
    do {
        *dst = val;
        val += 0x4000;
        dst++;
    } while (val < (s32)0x801A0000);

    switch (step_mode) {
    case 0:
        if (now_girl >= 13U) {
            goto_step(196);
            return;
        }
        back_clear_g = 255;
        back_clear_r = 255;
        back_clear_b = 255;
        back_clear_switch(1);
        draw2d3d(1, 0);
        prim_init();
        class_clear_f();
        cnt = 0;
        last = 15;
        if (pad_data & 0x800) {
            cd_read_start2(49, 0x80180000, etc_bukatu_lba_tbl[13]);
            step_mode = step_mode + 2;
        } else if (now_year == 98 && now_month == 3 && now_day == 1) {
            i = 0;
            q = album_work;
            p = screen_w;
            do {
                c = p[1340];
                if (c != 0 || q[20] != 0) {
                    printf(etc_bukatu_clear_fmt, i, c, q[20]);
                    cnt = cnt + 1;
                } else {
                    printf(etc_bukatu_nonclear_fmt, i, last);
                    last = i;
                }
                i = i + 1;
                p = p + 1;
                q = q + 1;
            } while (i != 13);
            v = now_girl;
            if (last == (v & 15) && cnt >= 12) {
                cd_read_start2(49, 0x80180000, etc_bukatu_lba_tbl[13]);
                step_mode = step_mode + 2;
            } else {
                cd_read_start2(33, 0x80180000, etc_bukatu_lba_tbl[v]);
                step_mode = step_mode + 1;
            }
        } else {
            v = now_girl;
            cd_read_start2(33, 0x80180000, etc_bukatu_lba_tbl[v]);
            step_mode = step_mode + 1;
        }
        printf(etc_bukatu_result_str0);
        break;

    case 1:
        if ((get_CD_State2() & 1) == 0) {
            break;
        }
        disp_mode_set(1);
        bg_set(buf, 0, 4, 2, 2, 511, -128, -120, 8, 9);
        palette_upload_enqueue(0x80190000, 31, 1, 3, 0);
        sub_step_up();
        goto_sub_sub_step(0);
        break;

    case 2:
        if ((get_CD_State2() & 1) == 0) {
            break;
        }
        bg_set(buf, 0, 6, 3, 2, 511, -160, -120, 8, 9);
        palette_upload_enqueue(0x80198000, 31, 1, 3, 0);
        sub_step_up();
        set_XAsector(0x1D000077, 0x1000007B, 0);
        device_cmd_send(768);
        goto_sub_sub_step(1);
        break;
    }
    white_light(255);
}
