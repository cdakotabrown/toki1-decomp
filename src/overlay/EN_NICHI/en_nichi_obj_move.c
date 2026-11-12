/* en_nichi_obj_move - EN_NICHI overlay, Address: 0x80133B50, Size: 724 bytes */
/* @SymbolStart: 0x80133B50 */
/* @SymbolSize: 724 */
/* @RodataBase: 0x80139478 */
/* @PostProc: patch_stack_complete --old 64 --new 48 */
/* @PostProc: remap_stack_slots --map 28:40 */
/* @PostProc: patch_insn_reg_field 0x28:rt:v0:t8 0x2c:rt:a1:t0 0x38:rs:v0:t8 0x38:rt:t8:t9 0x40:rs:a1:t0 0x44:rt:t8:t9 0x58:rd:t0:t2 0x5c:rt:t9:t1 0x60:rd:t1:t3 0x60:rt:t0:t2 0x68:rd:t2:t4 0x68:rs:t9:t1 0x68:rt:t1:t3 0x70:rt:t2:t4 0x78:rd:t4:t6 0x7c:rt:t3:t5 0x80:rd:t5:t7 0x80:rt:t4:t6 0x84:rd:t6:t8 0x84:rs:t3:t5 0x84:rt:t5:t7 0x8c:rt:t6:t8 0x90:rt:t7:t9 0x98:rs:t7:t9 0xa0:rd:t7:t9 0xa0:rt:t7:t9 0xa8:rt:t7:t9 0xac:rt:t7:t9 0xb4:rs:t7:t9 0xc8:rt:t8:t0 0xd0:rt:t8:t0 0xd4:rt:t0:t2 0xd8:rs:t0:t2 0xd8:rt:t0:t2 0xe0:rt:t0:t2 0xe4:rt:t9:t1 0xe8:rs:t9:t1 0xe8:rt:t9:t1 0xf0:rt:t9:t1 0xf4:rt:t1:t3 0xf8:rs:t1:t3 0xf8:rt:t1:t3 0x100:rt:t1:t3 0x104:rt:t2:t4 0x108:rs:t2:t4 0x108:rt:t2:t4 0x110:rt:t2:t4 0x114:rt:t4:t6 0x118:rs:t4:t6 0x118:rt:t4:t6 0x120:rt:t4:t6 0x124:rt:t3:t5 0x128:rs:t3:t5 0x128:rt:t3:t5 0x130:rt:t3:t5 0x134:rt:t5:t7 0x138:rs:t5:t7 0x138:rt:t5:t7 0x13c:rt:t5:t7 0x16c:rt:t7:t9 0x174:rs:t7:t9 0x174:rt:t8:t0 0x17c:rt:t8:t0 0x190:rt:t1:t3 0x194:rt:t0:t2 0x19c:rs:t0:t2 0x19c:rt:t9:t1 0x1a0:rt:t9:t1 0x1a4:rt:t1:t3 0x1a8:rs:t1:t3 0x1b0:rt:t2:t4 0x1b8:rs:t2:t4 0x1b8:rt:t4:t6 0x1bc:rt:t4:t6 0x1c0:rt:t3:t5 0x1c4:rs:t3:t5 0x1cc:rt:t5:t7 0x1d4:rs:t5:t7 0x1d4:rt:t6:t8 0x1d8:rt:t6:t8 0x204:rt:t7:t9 0x20c:rs:t7:t9 0x20c:rt:t8:t0 0x214:rt:t8:t0 0x218:rt:t0:t2 0x220:rs:t0:t2 0x220:rt:t9:t1 0x228:rt:t9:t1 0x22c:rt:t1:t3 0x234:rs:t1:t3 0x234:rt:t2:t4 0x23c:rt:t2:t4 0x240:rt:t4:t6 0x248:rs:t4:t6 0x248:rt:t3:t5 0x24c:rt:t3:t5 0x260:rt:v0:t7 0x264:rt:v0:t7 0x268:rs:v0:t7 0x270:rt:v0:t8 0x274:rt:v0:t8 0x278:rs:v0:t8 0x28c:rt:t6:t0 0x290:rt:t5:t9 0x294:rs:t6:t0 0x29c:rd:t7:t2 0x2a0:rd:t8:t1 0x2a0:rt:t7:t2 0x2a4:rd:t0:t3 0x2a4:rs:t5:t9 0x2a4:rt:t8:t1 0x2ac:rt:t0:t3 0x2b8:rt:t1:t6 0x2bc:rt:t9:t4 0x2c0:rs:t1:t6 0x2c4:rd:t2:t5 0x2c8:rd:t4:t7 0x2c8:rt:t2:t5 0x2cc:rd:t3:t8 0x2cc:rs:t9:t4 0x2cc:rt:t4:t7 0x2d0:rt:t3:t8 */
#include "types.h"

typedef struct {
    s32 kind;
    s32 u04;
    s32 u08;
    s32 u0C;
    s32 u10;
    s32 x;
    s32 y;
    s32 ang;
    s32 spd;
    s32 tmr;
    s32 sel;
    s32 inp;
} EnNichiObj;

extern EnNichiObj en_nichi_obj_wk[];

extern s32 rcos(s32);
extern s32 rsin(s32);

void en_nichi_obj_move(s32 idx)
{
    EnNichiObj *p;
    s32 tmr;
    s32 kind;
    s32 m;
    s32 a;
    s32 t;
    s32 r;

    p = &en_nichi_obj_wk[idx];
    tmr = p->tmr;
    kind = p->kind;
    p->tmr = tmr - 1;

    if (kind == 4) {
        r = rcos(p->ang);
        p->x += (r << 15) >> 12;
        r = rsin(p->ang);
        p->y += (r << 15) >> 12;
        return;
    }

    switch ((u32)p->sel) {
    case 0:
        p->spd = 0xC000;
        break;
    case 1:
        p->spd = 0xF000;
        break;
    case 2:
        p->spd = 0x12000;
        break;
    case 3:
        p->spd = 0x15000;
        break;
    case 4:
        p->spd = 0x18000;
        break;
    case 5:
        p->spd = 0x1B000;
        break;
    case 6:
        p->spd = 0x1E000;
        break;
    case 7:
        p->spd = 0x21000;
        break;
    case 8:
        p->spd = 0x24000;
        break;
    }

    if (p->inp == 1) {
        m = p->tmr % 40;
        if (m < 10) {
            p->ang += 30;
        } else if (m >= 20) {
            if (m < 30) {
                p->ang -= 30;
            }
        }
    }
    if (p->inp & 0x10) {
        p->ang += 20;
    }
    if (p->inp & 0x20) {
        p->ang -= 20;
    }
    switch ((u32)p->inp) {
    case 64:
        p->spd = 0xC000;
        p->ang += 256;
        break;
    case 65:
        p->spd = 0xC000;
        p->ang += 512;
        break;
    case 128:
        p->spd = 0xC000;
        p->ang -= 256;
        break;
    case 129:
        p->spd = 0xC000;
        p->ang -= 512;
        break;
    }

    a = p->ang;
    if (a >= 4096) {
        t = a - 4096;
        p->ang = t;
        a = t;
    }
    if (a < 0) {
        t = a + 4096;
        p->ang = t;
        a = t;
    }

    r = rcos(a);
    p->x += (s32)((u32)p->spd * (u32)r) >> 12;
    r = rsin(p->ang);
    p->y += (s32)((u32)p->spd * (u32)r) >> 12;
}
