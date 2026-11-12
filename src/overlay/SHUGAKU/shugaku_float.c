/* @SymbolStart: 0x80136058 */
/* @SymbolSize: 708 */
/* @CompileEnv: IDO52_AS1_TIE_SRC_ORDER=bb:3,5 IDO52_AS1_TIE_REJECT=22,68 */
/* @PostProc: patch_insn_reg_field 0x78:rt:t2:t4 0x7c:rt:t2:t4 0x90:rt:t3:t5 0x94:rs:t3:t5 0x94:rt:t3:t5 0x9c:rs:t3:t5 0x9c:rt:t4:t6 0xa0:rt:t4:t6 0xa8:rt:t5:t7 0xac:rt:t5:t7 0xb0:rt:t6:t8 0xb4:rs:t6:t8 0xb4:rt:t6:t8 0xbc:rt:t6:t8 0xc0:rt:t7:t9 0xc4:rs:t7:t9 0xc4:rt:t7:t9 0xcc:rt:t7:t9 0xd0:rt:v0:t1 0xd4:rs:v0:t1 0xd4:rt:v0:t1 0xd8:rt:t9:t3 0xdc:rs:v0:t1 0xdc:rt:t8:t2 0xe0:rs:t8:t2 0xe8:rs:t9:t3 0xe8:rt:t9:t3 0xf0:rs:t9:t3 0xf4:rt:t1:t4 0xf8:rs:t1:t4 0xf8:rt:t1:t4 0x100:rs:t1:t4 0x100:rt:t2:t5 0x104:rt:t2:t5 0x108:rt:t3:t6 0x10c:rs:t3:t6 0x10c:rt:t3:t6 0x114:rd:t4:t7 0x114:rs:t3:t6 0x114:rt:t2:t5 0x118:rt:t4:t7 0x11c:rt:t5:t8 0x120:rs:t5:t8 0x120:rt:t5:t8 0x128:rs:t5:t8 0x128:rt:t6:t9 0x12c:rt:t6:t9 0x130:rt:t7:t2 0x134:rs:t7:t2 0x134:rt:t7:t2 0x13c:rd:t8:t3 0x13c:rs:t7:t2 0x13c:rt:t6:t9 0x140:rt:t8:t3 0x144:rt:t9:t4 0x148:rs:t9:t4 0x148:rt:t9:t4 0x14c:rs:v0:t1 0x14c:rt:t1:t6 0x150:rs:t9:t4 0x15c:rt:t1:t6 0x160:rt:v0:t5 0x164:rs:v0:t5 0x164:rt:v0:t5 0x168:rt:t3:t7 0x16c:rs:v0:t5 0x174:rt:t2:t8 0x17c:rs:t2:t8 0x17c:rt:t2:t8 0x180:rt:t3:t7 0x188:rs:t2:t8 0x198:rt:t4:t2 0x19c:rs:t4:t2 0x19c:rt:t4:t2 0x1a4:rs:t4:t2 0x1b4:rt:t5:t9 0x1b8:rs:t5:t9 0x1b8:rt:t5:t9 0x1c0:rs:t5:t9 0x1c4:rt:t7:t3 0x1c8:rs:t7:t3 0x1c8:rt:t7:t3 0x1d0:rs:t7:t3 0x1e0:rt:v0:t4 0x1e4:rs:v0:t4 0x1e4:rt:v0:t4 0x1ec:rs:v0:t4 0x1ec:rt:v0:t1 0x1f0:rt:v0:t1 0x1f4:rt:v0:t1 0x1f8:rd:t8:t5 0x1fc:rs:t8:t5 0x204:rt:t1:t0 0x208:rt:t9:t7 0x210:rt:t9:t7 0x214:rs:t1:t0 0x25c:rd:t6:t1 0x260:rd:t8:t6 0x264:rd:t9:t5 0x264:rs:t6:t1 0x264:rt:t8:t6 0x268:rd:t1:t7 0x268:rs:t9:t5 0x270:rt:t5:t3 0x274:rt:t7:t4 0x278:rt:t7:t4 0x27c:rt:t5:t3 0x28c:rd:t3:t8 0x290:rt:t3:t8 0x29c:rd:t3:t8 0x29c:rs:t1:t7 0x2a0:rt:t3:t8 0x30:rt:v0:t9 0x40:rs:v0:t9 0x40:rt:v0:t9 0x48:rs:v0:t9 0x48:rt:v0:t1 0x4c:rd:t9:t2 0x4c:rt:v0:t1 0x50:rt:v0:t1 0x54:rd:t1:t3 0x54:rt:t9:t2 0x58:rs:t1:t3 */
#include "types.h"

extern s16 cw99_anim;

extern u8  cw99_anim_flag;
extern volatile u8 cw99_disp;
extern s16 cw98_tick;
extern s16 cw98_frame;
extern s16 cw99_tick;
extern s16 cw99_anim;
extern s16 cw99_frame;
extern s32 cw99_x_fix;
extern volatile s16 cw99_x;
extern s32 cw99_y_fix;
extern s32 cw99_vx;
extern s32 cw99_vy;
extern u8  fade_bri;
extern s16 shugaku_float_cnt;
extern u8  update_anim_state_tbl4;

extern void device_cmd_send(s32 code);
extern void shugaku_cw_disp_off(void);
extern void shugaku_cw_anim_set2(void);
extern void iris_show(s32 a0, s32 a1, s32 a2, s32 a3,
                          s32 arg4, s32 arg5, s32 arg6, s32 arg7);
extern void dtd_on_tpage(s32 a0, s32 a1, s32 a2, s32 a3, s32 arg4);

void shugaku_float(void)
{
    s32 sel;
    u32 flags;
    s32 vel;
    s32 mode;
    s32 tmp;
    s16 count;
    s32 bright;
    u32 lvl;

    sel = cw99_anim;

    if (sel == 5 && !(cw99_disp & 0x80)) {
        tmp = shugaku_float_cnt + 1;
        shugaku_float_cnt = tmp;
        if ((s16)tmp >= 41) {
            device_cmd_send(1280);
            shugaku_float_cnt = 0;
            cw99_anim = 6;
            cw99_frame = 0;
            cw99_tick = 0;
            cw99_disp |= 0x80;
            cw99_anim_flag = 1;
            cw99_vx = 0x00064000;
            cw99_vy = 0x00012000;
        }
    }

    flags = cw99_disp;
    if (flags & 0x80) {
        if (cw99_anim == 6) {
            cw99_vx -= 6144;
            cw99_x_fix += cw99_vx;
            cw99_vy -= 1536;
            cw99_y_fix += cw99_vy;
            if (cw99_x < -160) {
                cw99_disp = flags & ~0x80;
            }
        }
    }

    vel = cw99_x;
    if (vel < -160) {
        update_anim_state_tbl4 = 9;
        if (shugaku_float_cnt == 100) {
            shugaku_cw_disp_off();
        }
        if (shugaku_float_cnt == 10) {
            device_cmd_send(1282);
        }
        if (cw98_frame == 1 && cw98_tick == 33) {
            shugaku_cw_anim_set2();
        }
        tmp = shugaku_float_cnt + 1;
        shugaku_float_cnt = tmp;
        if ((s16)tmp >= 166) {
            shugaku_float_cnt = 165;
        }
        count = shugaku_float_cnt;
        bright = count + 128;
        mode = 37;
        if ((u32)bright >= 256U) {
            bright = 255;
        }
        if (bright == 255) {
            lvl = fade_bri;
            bright = lvl << 1;
            if (lvl == 0) {
                bright = 0;
            } else {
                bright = bright - 1;
            }
        }
        iris_show(mode, 0, 511, (s16)(((u32)(count * count)) >> 1),
                      -160, -120,
                      ((bright << 16) | (bright << 8) | bright) | 0x10000000,
                      10);
        dtd_on_tpage(0, 0, 10, 0, 0);
    }
}
