/* Address: 0x8013A318 */
/* Size: 380 bytes (95 instructions) */
#include "types.h"

extern s8 D_801218DC[];
extern s32 D_801203B8;
extern s32 D_801203D0;
extern s32 D_801203E8;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_8012183C;
extern s32 D_80121840;
extern s32 D_80121844;
extern s32 D_80121848;
extern s32 D_8012184C;
extern s32 D_80121850;
extern s32 D_80121854;
extern s32 D_80121830;
extern s32 D_80121834;
extern s32 D_8012182C;
extern s16 D_80121838;
extern s32 D_800EECEC;
extern volatile s16 D_800B0AA2;
extern volatile s16 D_800B0AA6;
extern volatile s16 D_800B0AAA;
extern s16 D_800EAFB8;

extern s32 func_800789A0(void);
extern void func_800FDCD0(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s32);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_8013A318(void) {
    s16 i;

    for (i = 0; i < 12; i++) {
        D_801218DC[i] = (s8)(func_800789A0() % 4);
    }
    D_80120678 = D_801203B8;
    D_8012067C = D_801203D0;
    D_80120680 = D_801203E8;
    func_800FDCD0();
    func_80012D64(D_8012183C, 17, 1, 2, 0);
    func_8004C46C(D_80121840, D_80121844, D_80121848, D_8012184C,
                  D_80121850, D_80121854);
    func_8004C6B0(D_80121830, D_80121834, D_8012182C, D_80121838);
    D_800EECEC = 2;
    D_800EAFB8 = 3;
    D_800B0AA2 += 2;
    D_800B0AA6 += 1;
    D_800B0AAA -= 20;
    func_8004C250();
    func_80011DFC();
}
