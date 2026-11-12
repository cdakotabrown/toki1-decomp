/* @CompileFlags: -Wc,-pic0 -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=47:t4,44:v0,166:v1,28:a3,32:a1,37:a2 */
/* @PostProc: patch_insn_reg_field 0xac:rs:v0:v1 0xac:rt:v1:v0 0x19c:rs:v0:v1 0x19c:rt:v1:v0 */
/* Address: 0x80153D80 */
/* Size: 656 bytes (164 instructions) */
#include "types.h"

extern s16 *D_8009471C;
extern s16 *D_80094720;
extern s32 D_80094724;
extern s32 D_80094728;
extern s32 D_8009472C;

extern s32 D_801241D8;
extern s32 D_801241E8;
extern s32 D_801241F8;

extern s32 D_800EECBC;
extern s16 D_800B095E;
extern u8 D_8012531C;
extern u8 D_801243B0;
extern u8 D_800B1746;

extern s16 D_8012438C;
extern s16 D_80124394;
extern s16 D_8012439C;
extern s16 D_801243A4;

extern void func_8004A8EC(s32);
extern void func_80033E88(void);
extern s32 func_80049B20(s32, s32, s32);
extern void func_80011DFC(void);

void func_80153D80(void) {
    s32 sceneState;
    s16 resultShort;
    s16 modeShort;
    u8 charId;
    u8 sceneFlag;
    u8 sceneId;
    s16 dayCount;
    s32 ret;

    D_8009471C = &resultShort;
    D_80094724 = D_801241D8;
    D_80094728 = D_801241E8;
    D_8009472C = D_801241F8;
    resultShort = 0;
    sceneState = D_800EECBC;

    if (sceneState == 0 && D_800B095E < 40 && D_8012531C == 1 &&
        D_801243B0 == 16) {
        charId = D_800B1746;
        if (charId == 0 || charId == 1 || charId == 3 || charId == 4 ||
            charId == 5 || charId == 8 || charId == 9) {
            D_8012439C = 3;
            D_8012438C = 3;
            D_80124394 = 3;
            D_801243A4 = 2;
            func_8004A8EC(3);
            func_80033E88();
            modeShort = 5;
            D_80094720 = &modeShort;
            ret = func_80049B20(D_800EECBC, 0, 0);
            if (ret != 0) {
                func_80011DFC();
            }
            return;
        }
    }

    dayCount = D_800B095E;
    sceneId = D_801243B0;
    sceneFlag = D_8012531C;
    charId = D_800B1746;

    if ((sceneState == 0 && dayCount < 40 && sceneFlag == 0 &&
         sceneId == 19 &&
         (charId == 0 || charId == 1 || charId == 3 || charId == 6 ||
          charId == 7)) ||
        (sceneState == 0 && dayCount < 100 && sceneFlag == 0 &&
         sceneId == 20 &&
         (charId == 0 || charId == 4 || charId == 6 || charId == 7 ||
          charId == 8 || charId == 10))) {
        D_8012439C = 3;
        D_8012438C = 3;
        D_80124394 = 3;
        D_801243A4 = 2;
        func_8004A8EC(3);
        func_80033E88();
        modeShort = 6;
        D_80094720 = &modeShort;
        ret = func_80049B20(D_800EECBC, 0, 0);
        if (ret != 0) {
            func_80011DFC();
        }
        return;
    }

    func_80011DFC();
}
