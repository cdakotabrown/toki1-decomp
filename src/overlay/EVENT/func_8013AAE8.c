/* @PostProc: patch_stack_complete --old 216 --new 224 */
/* Address: 0x8013AAE8 */
/* Size: 736 bytes (184 instructions) */
#include "types.h"

typedef struct {
    s16 lo;
    s16 hi;
} EvtRange;

typedef struct {
    EvtRange r[41];
} EvtRangeTbl;

typedef struct {
    s32 a;
    s32 b;
} EvtPair;

extern EvtPair D_80121A2C;
extern EvtRangeTbl D_80121A34;

extern u32 D_800B1AF0;
extern s32 D_800B176C;
extern s16 D_800EAFB8;

extern s32 func_80015A0C();
extern s32 func_80015A64();
extern void func_80015C34(s32, s32, s32);
extern void func_800433D0(s32);
extern void func_80011DFC();
extern void func_800FEDC8();

void func_8013AAE8(void) {
    EvtPair p;
    EvtRangeTbl tbl;
    u32 val;
    s16 i;
    s32 inRange;
    s32 handled;

    p = D_80121A2C;
    tbl = D_80121A34;

    if (D_800B1AF0 == 0) {
        func_80015C34(p.a, p.b, 15);
        func_800433D0(113);
        func_800433D0(768);
        D_800B1AF0++;
    } else if (func_80015A0C() == 9) {
        func_80011DFC();
        func_800433D0(514);
    } else if (D_800B176C & 0x00400040) {
        func_80011DFC();
        func_800433D0(180);
        func_800433D0(514);
    } else {
        val = D_800B1AF0;
        inRange = 0;
        handled = 0;
        for (i = 0; i < 41; i++) {
            if (val < tbl.r[i].lo) {
                continue;
            }
            if (tbl.r[i].hi < val) {
                continue;
            }
            inRange = 1;
            if (i == 6) {
                D_800EAFB8 = 2;
                handled = 1;
            } else if (i == 12) {
                D_800EAFB8 = 2;
                handled = 1;
            } else if (i == 22) {
                D_800EAFB8 = 0;
                handled = 1;
            } else if (i == 23) {
                D_800EAFB8 = 1;
                handled = 1;
            } else if (i == 24) {
                D_800EAFB8 = 2;
                handled = 1;
            } else if (i == 25) {
                D_800EAFB8 = 2;
                handled = 1;
            } else if (i == 26) {
                D_800EAFB8 = 0;
                handled = 1;
            } else if (i == 29) {
                D_800EAFB8 = 0;
                handled = 1;
            } else if (i == 33) {
                D_800EAFB8 = 0;
                handled = 1;
            } else if (i == 36) {
                D_800EAFB8 = 0;
                handled = 1;
            } else if (i == 40) {
                D_800EAFB8 = 0;
                handled = 1;
            }
        }
        if (inRange == 0) {
            D_800EAFB8 = 3;
        } else if (handled == 0) {
            func_800FEDC8();
        }
    }

    if (func_80015A64() & 4) {
        D_800B1AF0++;
    }
}
