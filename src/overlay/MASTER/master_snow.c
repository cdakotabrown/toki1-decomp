/* @PostProc: swap_instructions --offset1 0x278 --offset2 0x27C */
/* @PostProc: patch_insn_reg_field 0x140:rs:t8:s4 0x140:rt:s4:t8 0x180:rs:t4:s4 0x180:rt:s4:t4 0x1ac:rs:t0:s4 0x1ac:rt:s4:t0 0x278:rs:s6:s5 0x278:rt:s6:s5 0x27c:rs:s5:s6 0x27c:rt:s5:s6 0x28c:rd:t4:t5 0x290:rs:s5:s6 0x290:rt:t4:t5 0x298:rs:t6:s4 0x298:rt:s4:t6 0x2ac:rd:t5:t4 0x2b0:rs:s6:s5 0x2b0:rt:t5:t4 */
/* Address: 0x80137BF4, Size: 928 bytes (232 instructions) */
#include "types.h"

typedef struct {
    u8  f0;
    u8  f1;
    u8  f2;
    u8  f3;
    u8  f4;
    u8  f5;
    u8  f6;
    u8  f7;
    s16 f8;
    s16 f10;
    s32 f12;
    s32 f16;
    s16 f20;
    s16 f22;
    s16 f24;
    u8  pad26[12];
    s16 f38;
    s16 f40;
    s16 f42;
    u8  pad44[8];
    s32 f52;
    s32 f56;
    u8  pad60[7];
    u8  f67;
} Blk;

extern Blk class_w[];
extern u8  cal_cw_mode;
extern s16 cal_cw_tick;
extern u8  cal_cw_bright;
extern s16 cal_cw_anim;
extern s16 cal_cw_frame;
typedef struct { s16 x; s16 y; } Flake;
extern Flake master_snow_xy[];
extern s16 master_snow_spd[];

extern s32  rand(void);
extern void line_set(s32, s32, s16, s32, s32, s32, s32);

void master_snow(void) {
    s16 i;
    s16 v;
    Blk *p;
    Flake *q;

    if (cal_cw_anim == 0 && cal_cw_frame == 7 && cal_cw_tick == 2) {
        cal_cw_anim = 1;
        cal_cw_frame = 0;
        cal_cw_tick = 0;
        cal_cw_mode = 1;
    }
    if (cal_cw_anim == 2 && cal_cw_frame == 1 && cal_cw_tick == 4) {
        cal_cw_anim = 3;
        cal_cw_frame = 0;
        cal_cw_tick = 0;
        cal_cw_mode = 1;
    }

    for (i = 97; i < 116; i++) {
        p = &class_w[i];
        v = p->f24;
        if (v < 2 || v >= 8) {
            *(s32 *)&p->f40 = *(s32 *)&p->f40
                - (cal_cw_anim == 3) * 327680 - 327680;
        } else if (v == 4 || v == 7) {
            *(s32 *)&p->f40 = *(s32 *)&p->f40
                - (cal_cw_anim == 3) * 196608 - 196608;
        } else {
            *(s32 *)&p->f40 = *(s32 *)&p->f40
                - (cal_cw_anim == 3) * 131072 - 131072;
        }
        if (p->f42 < -159) {
            p->f38 = rand() % 320 - 160;
            p->f42 = 60;
            p->f24 = rand() % 10;
        }
    }

    if (cal_cw_bright >= 120U) {
        for (i = 0; i < 10; i++) {
            v = master_snow_spd[i];
            q = &master_snow_xy[i];
            q->y = q->y - (v * 4) * ((cal_cw_anim == 3) + 1);
            line_set(q->x, q->y, q->x,
                          (s16)(v * 20 + q->y), 0xFFFFFF, 0xFFFFFF, 11);
            if (q->y < -170) {
                q->y = 60;
                q->x = rand() % 320 - 160;
                master_snow_spd[i] = rand() % 5 + 1;
            }
        }
    }
}
