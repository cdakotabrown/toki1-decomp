/* @RodataBase: 0x801202A8 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u32 time;
    s32 unk4;
    s32 unk8;
    s32 step;
    u16 unk10;
    u8 who;
} Scene;
extern Scene D_800B1AE4;
extern s32 ev_flag3c;
extern u16 D_801258DC;
s16 D_801258E4[4] = {0};
s16 D_801258EC[2] = {0};
s16 D_801258F0[2] = {0};
extern s16 D_801258F4[];
extern s16 D_801258FC[];

extern void func_8011F530();
extern void func_800433D0();
extern void func_800352B4();
extern void func_800469F4();
extern s32  func_800789A0();
extern s32  func_80015A44();
extern void func_8002755C();
extern void func_80027438();
extern void func_80046480();
extern void func_80046598();
extern s32 func_8001BB10();
extern void func_80011DFC();

void func_8015ADFC()
{
    s16 i;
    u8 who;
    s32 x;
    s32 step;

    switch (D_800B1AE4.step) {
    case 0:
        ev_flag3c = 1;
        func_8011F530(D_800B1AE4.time);
        func_800433D0(514);
        D_801258DC = 0;
        func_800352B4(0);
        func_800469F4(17194);
        D_801258E4[0] = -80;
        D_801258E4[1] = 0;
        D_801258E4[2] = 100;
        D_801258EC[0] = -40;
        D_801258EC[1] = 0;
        D_801258F0[0] = -60;
        D_800B1AE4.step++;
        D_801258F4[0] = func_800789A0() % 3 - 1;
        D_801258F4[1] = func_800789A0() % 3 - 1;
        D_801258F4[2] = func_800789A0() % 3 - 1;
        D_801258FC[0] = func_800789A0() % 3 - 1;
        D_801258FC[1] = func_800789A0() % 3 - 1;
        D_801258FC[2] = func_800789A0() % 3 - 1;
        break;
    case 1:
        if (func_80015A44() & 1) {
            D_800B1AE4.step++;
        }
        break;
    case 2:
        func_8002755C(1);
        func_80027438(1);
        D_800B1AE4.step++;
        break;
    case 3:
        who = D_800B1AE4.who;
        step = D_800B1AE4.step;
        func_80046480();
        D_800B1AE4.step = step;
        if (who != D_800B1AE4.who) {
            D_800B1AE4.step++;
            func_800433D0(513);
        }
        D_800B1AE4.who = who;
        break;
    case 4:
        for (i = 0; i < 3; i++) {
            if (D_800B1AE4.time % 30 == func_800789A0() % 30) {
                D_801258F4[i] = func_800789A0() % 3 - 1;
            }
            if (D_800B1AE4.time % 30 == func_800789A0() % 30) {
                D_801258FC[i] = func_800789A0() % 3 - 1;
            }
            if (D_801258F4[i] == 0 && D_801258FC[i] == 0) {
                if (func_800789A0() % 2) {
                    D_801258F4[i] = func_800789A0() % 2 * 2 - 1;
                } else {
                    D_801258FC[i] = func_800789A0() % 2 * 2 - 1;
                }
            }
            D_801258E4[i] += D_801258F4[i];
            D_801258EC[i] += D_801258FC[i];
            if (D_801258E4[i] < -149) {
                D_801258F4[i] = 1;
            }
            if (D_801258E4[i] >= 150) {
                D_801258F4[i] = -1;
            }
            if (D_801258EC[i] < -109) {
                D_801258FC[i] = 1;
            }
            if (D_801258EC[i] >= 30) {
                D_801258FC[i] = -1;
            }
        }
        x = func_8001BB10(D_801258E4[0], D_801258EC[0], 10, 2, 2, 0x808080, 1, 1);
        x = func_8001BB10(D_801258E4[1], D_801258EC[1], 10, 2, 2, 0x804040, 1, 1);
        x = func_8001BB10(D_801258E4[2], D_801258EC[2], 10, 2, 2, 0x800000, 1, 1);
        if (D_801258DC++ == 300) {
            D_800B1AE4.step++;
            func_800433D0(514);
        }
        break;
    case 5:
        who = D_800B1AE4.who;
        step = D_800B1AE4.step;
        func_80046598();
        D_800B1AE4.step = step;
        if (who != D_800B1AE4.who) {
            D_800B1AE4.step++;
        }
        D_800B1AE4.who = who;
        break;
    case 6:
        func_80027438(1);
        D_800B1AE4.step++;
        break;
    default:
        func_800433D0(518);
        ev_flag3c = 0;
        func_800352B4(1);
        func_80011DFC();
        break;
    }
}
