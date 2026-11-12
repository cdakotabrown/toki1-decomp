/* @PostProc: patch_stack_complete --old 112 --new 120 */
/* @PostProc: remap_stack_slots --map 80:84 */
/* Address: 0x80134C2C */
/* Size: 2000 bytes (500 instructions) */
#include "types.h"

typedef struct {
    s32 x;
    s32 y;
} PosXY;

typedef struct {
    PosXY p[4];
} PosTable;

extern PosTable D_80120A74;
extern u32 D_800B1AE4;
extern s16 D_801207AC;
extern s16 D_801207B0;

extern void func_800F9730(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g,
                          s32 h, s32 i, s32 j, s32 k, s32 l);
extern void func_8001A0CC(s32 a, s32 b, s32 c, s32 d, s32 e);

void func_80134C2C(void) {
    PosTable buf;
    u32 clock;
    u32 v0;
    s32 idx;

    buf = D_80120A74;

    clock = D_800B1AE4;

    if ((clock % 120) == 0) {
        D_801207AC = D_801207AC + 1;
    }
    if (D_801207AC >= 3) {
        D_801207AC = 0;
    }

    if ((clock % 100) == 0) {
        D_801207B0 = D_801207B0 + 1;
    }
    if (D_801207B0 >= 4) {
        D_801207B0 = 0;
    }

    v0 = clock % 36;
    if (v0 < 12) {
        idx = D_801207B0;
        func_800F9730(160, -160, 180, -160, 110, 40, 130, 40, buf.p[idx].x,
                      buf.p[idx].y, 9, 1);
        idx = D_801207B0;
        func_800F9730(165, -160, 195, -160, 100, 40, 70, 40, buf.p[idx].x,
                      buf.p[idx].y, 9, 1);
        idx = D_801207B0;
        func_800F9730(170, -160, 180, -160, 65, 40, 50, 40, buf.p[idx].x,
                      buf.p[idx].y, 9, 1);
        idx = D_801207B0;
        func_800F9730(160, -160, 175, -160, 105, 40, 125, 40, buf.p[idx].x,
                      buf.p[idx].y, 9, 1);
    } else if (v0 < 24) {
        idx = D_801207B0;
        func_800F9730(155, -160, 175, -160, 105, 40, 130, 40, buf.p[idx].x,
                      buf.p[idx].y, 9, 1);
        idx = D_801207B0;
        func_800F9730(155, -160, 195, -150, 90, 40, 60, 40, buf.p[idx].x,
                      buf.p[idx].y, 9, 1);
        idx = D_801207B0;
        func_800F9730(165, -160, 175, -160, 60, 40, 45, 40, buf.p[idx].x,
                      buf.p[idx].y, 9, 1);
        idx = D_801207B0;
        func_800F9730(150, -160, 160, -160, 100, 40, 115, 40, buf.p[idx].x,
                      buf.p[idx].y, 9, 1);
    }
    func_8001A0CC(0, 0, 9, 3, 0);

    clock = D_800B1AE4;
    v0 = clock % 30;
    if (v0 < 10) {
        idx = D_801207AC;
        func_800F9730(30, -160, 100, -160, -320, 40, -250, 40, buf.p[idx].x,
                      buf.p[idx].y, 13, 1);
        idx = D_801207AC;
        func_800F9730(10, -160, 25, -160, -220, 40, -190, 40, buf.p[idx].x,
                      buf.p[idx].y, 13, 1);
        idx = D_801207AC;
        func_800F9730(0, -160, 20, -160, -180, 40, -130, 40, buf.p[idx].x,
                      buf.p[idx].y, 13, 1);
        idx = D_801207AC;
        func_800F9730(25, -160, 40, -160, -300, 40, -280, 40, buf.p[idx].x,
                      buf.p[idx].y, 13, 1);
    } else if (v0 < 20) {
        idx = D_801207AC;
        func_800F9730(20, -160, 90, -160, -340, 40, -270, 40, buf.p[idx].x,
                      buf.p[idx].y, 13, 1);
        idx = D_801207AC;
        func_800F9730(10, -160, 30, -160, -240, 40, -210, 40, buf.p[idx].x,
                      buf.p[idx].y, 13, 1);
        idx = D_801207AC;
        func_800F9730(0, -160, 15, -160, -200, 40, -150, 40, buf.p[idx].x,
                      buf.p[idx].y, 13, 1);
        idx = D_801207AC;
        func_800F9730(15, -160, 25, -160, -310, 40, -290, 40, buf.p[idx].x,
                      buf.p[idx].y, 13, 1);
    }
    func_8001A0CC(0, 0, 13, 3, 0);
}
