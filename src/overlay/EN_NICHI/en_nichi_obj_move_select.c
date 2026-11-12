/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: patch_stack_complete --old 88 --new 56 */
/* @PostProc: patch_insn_reg_field 0x184:rs:v1:a2 0x184:rt:a2:v1 0x1e8:rt:t1:t2 0x1ec:rt:v0:t1 0x1f4:rt:v0:t1 0x1f8:rt:t1:t2 0x204:rt:t5:t7 0x208:rt:t2:t3 0x20c:rs:t2:t3 0x25c:rt:t3:t4 0x268:rt:t3:t4 0x274:rt:t5:t7 0x278:rt:t4:t5 0x27c:rs:t4:t5 0x2e4:rt:t5:t7 0x2e8:rs:t5:t7 0x2ec:rt:t6:t8 0x2f0:rs:t6:t8 0x344:rt:a0:t9 0x34c:rs:a0:t9 0x364:rt:a0:t1 0x36c:rs:a0:t1 0x378:rt:a0:t3 0x380:rs:a0:t3 0x390:rs:v1:a2 0x390:rt:a2:v1 0x394:rt:t2:t0 0x3b0:rt:t9:t7 0x3b8:rs:t9:t7 0x3bc:rd:t7:t5 0x3c0:rs:t7:t5 0x3c0:rt:t8:t6 0x3c8:rt:t8:t6 0x3cc:rd:t0:t8 0x3cc:rs:t8:t6 0x3d0:rs:t0:t8 0x3d4:rd:t1:t9 0x3d4:rt:t0:t8 0x3d8:rs:t0:t8 0x3dc:rd:t1:t9 0x3e4:rt:t1:t9 0x3e8:rs:t2:t0 0x3ec:rt:t4:t2 0x3f0:rt:t3:t1 0x3f8:rt:t3:t1 0x3fc:rs:t4:t2 0x400:rt:t6:t4 0x404:rt:t5:t3 0x40c:rt:t5:t3 0x410:rs:t6:t4 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=16:6:3:3 */
/* @SymbolStart: 0x8013497C */
/* @SymbolSize: 1052 */
#include "types.h"

extern s32 rand(void);

extern u8 en_nichi_obj_wk[];

void en_nichi_obj_move_select(s32 idx)
{
    s32 *rec;
    s32 hundred;
    s32 v;
    s32 r;
    s32 x;
    s32 t;
    s32 y;
    s32 y2;
    s32 y3;

    rec = (s32 *)(en_nichi_obj_wk + idx * 48);
    if (rec[9] > 0) {
        return;
    }

    r = rand();
    hundred = 100;
    v = rec[2];
    switch (v) {
    case 0:
        x = r % hundred;
        if (x < 80) {
            rec[10] = 0;
        } else {
            rec[10] = 1;
        }
        break;
    case 1:
        x = r % hundred;
        if (x < 10) {
            rec[10] = 0;
        } else if (x < 90) {
            rec[10] = 1;
        } else {
            rec[10] = 2;
        }
        break;
    case 2:
        x = r % hundred;
        if (x < 20) {
            rec[10] = 1;
        } else {
            rec[10] = 2;
        }
        break;
    }

    r = rand();
    v = rec[11];
    if (v == 0 || v == 1) {
        x = r % hundred;
        if (x < 25) {
            rec[11] = 0;
        } else if (x < 40) {
            rec[11] = 1;
        } else if (x < 70) {
            rec[11] = 16;
        } else {
            rec[11] = 32;
        }
        v = rec[11];
    } else if (v & 0x50) {
        x = r % hundred;
        if (x < 40) {
            rec[11] = 0;
        } else if (x < 80) {
            rec[11] = 1;
        } else {
            rec[11] = 32;
        }
        v = rec[11];
    } else if (v & 0xA0) {
        x = r % hundred;
        if (x < 40) {
            rec[11] = 0;
            v = 0;
        } else if (x < 80) {
            rec[11] = 1;
            v = 1;
        } else {
            v = 16;
            rec[11] = v;
        }
    }

    if ((v & 0x10) || (v & 0x20)) {
        r = rand();
        x = r % hundred;
        if (x < 33) {
            y = rec[11];
            v = y | 1;
            rec[11] = v;
        } else if (x < 66) {
            y2 = rec[11];
            v = y2 | 2;
            rec[11] = v;
        } else {
            y3 = rec[11];
            v = y3 | 4;
            rec[11] = v;
        }
    }

    if (v == 0 || v == 1) {
        t = rand() % 25 + 40;
        rec[9] = t;
        if (rec[10] >= 4) {
            rec[9] = rec[9] / 4;
        }
    } else if (v & 1) {
        rec[9] = 25;
    } else if (v & 2) {
        rec[9] = 50;
    } else if (v & 4) {
        rec[9] = hundred;
    }
}
