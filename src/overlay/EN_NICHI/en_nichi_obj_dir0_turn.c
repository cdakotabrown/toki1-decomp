/* en_nichi_obj_dir0_turn - EN_NICHI overlay, Address: 0x80134EF0, Size: 440 bytes */
/* @SymbolStart: 0x80134EF0 */
/* @SymbolSize: 440 */
/* @PostProc: patch_insn_reg_field 0x1a0:rt:v1:t5 0x1a4:rt:v1:t5 0x1a8:rs:v1:t5 0x1b0:rt:t5:t6 0x1b4:rt:t5:t6 */
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

extern s32 rand(void);

void en_nichi_obj_dir0_turn(s32 idx, s32 dist)
{
    EnNichiObj *p;
    s32 v;
    s32 t;

    p = &en_nichi_obj_wk[idx];
    if (p->sel >= 3) {
        return;
    }

    switch (p->u04) {
    case 1:
        if (dist < 2116) {
            p->sel = p->u08 * 2 + 4;
            p->tmr = rand() % 15 + 30;
            v = rand();
            if ((v & 1) == 0) {
                p->inp = 0;
            } else {
                p->inp = 1;
            }
            if (dist >= 401) {
                p->tmr = 8;
                v = rand();
                if ((v & 1) == 0) {
                    p->inp = 128;
                } else {
                    p->inp = 64;
                }
            }
        }
        break;
    case 2:
        if (dist < 3844) {
            p->sel = p->u08 * 2 + 4;
            p->tmr = rand() % 15 + 30;
            v = rand();
            if ((v & 1) == 0) {
                p->inp = 0;
            } else {
                p->inp = 1;
            }
            if (dist >= 401) {
                p->tmr = 4;
                v = rand();
                if ((v & 1) == 0) {
                    p->inp = 129;
                } else {
                    p->inp = 65;
                }
            }
        }
        break;
    }

    v = p->ang;
    if (v >= 4096) {
        t = v - 4096;
        p->ang = t;
        v = t;
    }
    if (v < 0) {
        p->ang = v + 4096;
    }
}
