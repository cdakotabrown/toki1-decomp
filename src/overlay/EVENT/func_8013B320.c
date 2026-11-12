/* @PostProc: patch_stack_complete --old 104 --new 120 */
/* @PostProc: remap_stack_slots --map 60:76,72:80 */
/* Address: 0x8013B320 */
/* Size: 708 bytes (177 instructions) */
#include "types.h"

typedef struct {
    s32 a;
    s32 b;
} EvtPair;

typedef struct {
    EvtPair p[4];
} EvtPairTbl;

extern u8 D_801218DC[];
extern EvtPairTbl D_80121AD8;

extern void func_800FFA4C(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void func_8013B320(s16 arg0, s16 arg1, s16 arg2) {
    EvtPairTbl tbl;
    s32 x;
    s16 d;
    s16 e;

    tbl = D_80121AD8;
    x = arg0;

    if (x < 320) {
        d = x / 32 + 10;
        func_800FFA4C(320, -280, 160 - x, 40, 160 - x + d, 40,
                      tbl.p[D_801218DC[arg1]].a, tbl.p[D_801218DC[arg1]].b,
                      arg2, 1);
    } else if (x < 340) {
        d = (x - 320) * 7 / 8;
        func_800FFA4C(320, -280, -160, 40 - d, -160, 40,
                      tbl.p[D_801218DC[arg1]].a, tbl.p[D_801218DC[arg1]].b,
                      arg2, 1);
        d = x / 32 + 10;
        func_800FFA4C(320, -280, -160, 40, 160 - x + d, 40,
                      tbl.p[D_801218DC[arg1]].a, tbl.p[D_801218DC[arg1]].b,
                      arg2, 1);
    } else {
        d = (x - 320) * 7 / 8;
        e = 15 - (x - 320) / 32;
        func_800FFA4C(320, -280, -160, 40 - d, -160, 40 - d + e,
                      tbl.p[D_801218DC[arg1]].a, tbl.p[D_801218DC[arg1]].b,
                      arg2, 1);
    }
}
