/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:2:deny IDO52_AS1_NO_PASS1_PROP=27 IDO52_FORCE_WEB_COLOR=77:s5,78:s6 IDO52_UGEN_TEMP_REG=3:t7,14:t9 */
#include "types.h"

extern s32 system_config_disp();
extern void func_8008F618();
extern void func_80134938();
extern void func_8004DEAC();

typedef struct {
    u8 pad[430];
    s16 a;
    s16 pad2;
    s16 b;
    s16 pad3;
    s16 pad4;
    unsigned fpad:1;
    unsigned f1:1;
    unsigned fpad2:30;
} CharRec;

extern u8 D_800F53A0[];
extern u8 D_800F62CF;
extern s16 D_80145EB4;
extern s16 D_80145EB8;
extern u8 D_80145EC4;

#define REC(n) ((CharRec *)(D_800F53A0 + (n) * 56))

void func_801327D4()
{
    int i;
    int key;
    int r;
    int sel;
    u8 bestIdx;
    u8 bestScore;

    bestIdx = 255;
    bestScore = 4;
    for (i = 0; i < 11; i++) {
        if (REC(i)->f1) {
            key = system_config_disp(i);
            key &= 0x7F;
            if (key < bestScore
                || (key == bestScore
                    && REC(bestIdx)->b + REC(bestIdx)->a < REC(i)->a + REC(i)->b)) {
                bestIdx = i;
                bestScore = key;
            }
        }
    }
    r = system_config_disp(bestIdx);
    if (bestIdx != 255 && (unsigned)(r & 0x7F) < 2) {
        sel = bestIdx & 0xFF;
        D_800F62CF = sel;
        func_8008F618(sel);
        func_80134938();
        D_80145EB4 = 0;
        D_80145EB8 = 0;
        D_80145EC4 = 1;
        func_8004DEAC(1);
    } else {
        D_80145EC4 = 0;
        func_8004DEAC(2);
    }
}
