/* @CompileFlags: -Wc,-pic0 */
/* @RodataBase: 0x8015D4B0 */
/* @PostProc: remap_stack_slots --map '32:48,28:44' */
/* @PostProc: swap_commutative_operands --offset 0x90 --offset 0xFC --offset 0x160 --offset 0x1C4 */
/* @PostProc: swap_instructions --offset1 0x2AC --offset2 0x2C0 */
/* @PostProc: swap_instructions --offset1 0x2C0 --offset2 0x2EC */
/* @PostProc: swap_instructions --offset1 0x2E4 --offset2 0x2E8 */
/* @PostProc: patch_insn_reg_field 0x78:rs:t2:a0 0x78:rt:a0:t2 0xe4:rs:t0:a0 0xe4:rt:a0:t0 0x148:rs:t7:a0 0x148:rt:a0:t7 0x1ac:rs:t4:a0 0x1ac:rt:a0:t4 0x248:rt:t2:t1 0x2a4:rt:t1:t2 0x2ac:rt:t0:t9 0x2b0:rs:t2:t1 0x2b0:rt:t2:t1 0x2e4:rs:t0:t9 0x2e4:rt:t0:t9 0x2e8:rt:t9:t0 */
/* @SymbolStart: 0x8013240C */
/* @SymbolSize: 1188 */
/* Address: 0x8013240C (cross-prologue head) .. 0x801328B0 */
/* Size: 1188 bytes (297 insns incl. the 3 hoisted head words) */
#include "types.h"

typedef struct {
    u8  unk00[2];
    u8  f2;
    u8  f3;
    u8  unk04[100];
    s16 f104;
    s16 unk106[5];
    s16 f116;
    s16 f118;
    s16 f120;
    s16 f122;
    u8  unk7C[966];
    u8  f1090;
} Cam;

extern void taco_bg_show();

extern u32 pad_data;
extern u32 pad_trig;
extern Cam *taco_actor_rec;
extern u8 taco_turn_btn;

s32 taco_camera(void)
{
    Cam *p;
    Cam *q;
    s32 speed;
    s32 v;
    u32 c;
    u32 m;
    s32 sel;
    s32 maskA;
    s32 maskB;

    p = taco_actor_rec;

    speed = 32;
    if (((pad_data & 0xA000) != 0) && ((pad_data & 0x5000) != 0)) {
        speed = 45;
    }

    if ((pad_data & 0x8000) != 0) {
        if (taco_actor_rec->f116 >= 2049) {
            v = p->f116;
            p->f116 = v + ((speed * (4096 - v)) / 2048);
        } else {
            p->f116 = p->f116 + speed;
        }
    }

    if ((pad_data & 0x2000) != 0) {
        v = p->f116;
        if (v < -2048) {
            p->f116 = v + ((speed * (-4096 - v)) / 2048);
        } else {
            p->f116 = v - speed;
        }
    }

    if ((pad_data & 0x1000) != 0) {
        v = p->f118;
        if (v < -1792) {
            p->f118 = v + ((speed * (-2048 - v)) / 256);
        } else {
            p->f118 = v - speed;
        }
    }

    if ((pad_data & 0x4000) != 0) {
        v = p->f118;
        if (v >= 129) {
            p->f118 = v + ((speed * (384 - v)) / 256);
        } else {
            p->f118 = v + speed;
        }
    }

    if (p->f116 < -4096) {
        p->f116 = -4096;
    }
    if (p->f116 > 4096) {
        p->f116 = 4096;
    }
    if (p->f118 < -2048) {
        p->f118 = -2048;
    }
    if (p->f118 > 384) {
        p->f118 = 384;
    }

    switch ((u32)taco_turn_btn % 3U) {
    case 0:
        maskA = 5;
        maskB = 10;
        break;
    case 1:
        maskA = 4;
        maskB = 8;
        break;
    case 2:
        maskA = 1;
        maskB = 2;
        break;
    }

    if (p->f3 == 0) {
        if ((pad_trig & maskA) != 0) {
            q = taco_actor_rec;
            q->f1090++;
            p->f2 = 129;
            p->f3 = 127;
        }
        if ((pad_trig & maskB) != 0) {
            q = taco_actor_rec;
            q->f1090++;
            p->f2 = 130;
            p->f3 = 127;
        }
    }

    if ((p->f2 & 1) != 0) {
        c = p->f3;
        p->f104 = p->f104 + c;
        p->f116 = p->f116 + 8;
        if (c < 15) {
            if ((p->f104 & 0xFFF) >= 4065) {
                p->f3 = 0;
                p->f104 = 0;
                p->f2 = 0;
            }
        } else {
            p->f3 = c - 1;
            if (p->f3 < 64U) {
                p->f2 = p->f2 & 0x7F;
            }
        }
    }

    if ((p->f2 & 2) != 0) {
        c = p->f3;
        p->f104 = p->f104 - c;
        p->f116 = p->f116 - 8;
        if (c < 15) {
            if ((p->f104 & 0xFFF) < 31) {
                p->f3 = 0;
                p->f104 = 0;
                p->f2 = 0;
            }
        } else {
            p->f3 = c - 1;
            if (p->f3 < 64U) {
                p->f2 = p->f2 & 0x7F;
            }
        }
    }

    if ((p->f2 & 4) != 0) {
        q = taco_actor_rec;
        sel = q->f122--;
        switch ((u32)sel) {
        case 2:
        case 10:
        case 14:
        case 15:
            taco_bg_show();
            break;
        case 0:
            q = taco_actor_rec;
        q->f2 &= 0xFB;
            break;
        }
    }
}
