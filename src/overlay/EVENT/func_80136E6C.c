/* @PostProc: scheduler_normalizer */
/* Address: 0x80136E6C, Size: 272 bytes (68 instructions) */
#include "types.h"

typedef struct { u8 b[68]; } Buf68;
extern Buf68 D_800EAFA0;
extern Buf68 D_800EB028;
extern u8 D_800EAFE8;
extern u8 D_800EAFA4;
extern s16 D_800EB03E;

extern s32 D_80120CC4;
extern s32 D_80120CC8;
extern s32 D_80120CCC;
extern s16 D_80120CD0;
extern s32 D_80120CD4;
extern s32 D_80120CD8;
extern s32 D_80120CDC;
extern s32 D_80120CE0;
extern s32 D_80120CE4;
extern s32 D_80120CE8;
extern s32 D_80120CEC;

extern void func_800F9C4C(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_80011DFC(void);

void func_80136E6C(void) {
    func_800F9C4C();
    func_80012D64(D_80120CD4, 17, 1, 2, 0);
    func_8004C46C(D_80120CD8, D_80120CDC, D_80120CE0, D_80120CE4, D_80120CE8, D_80120CEC);
    func_8004C6B0(D_80120CC8, D_80120CCC, D_80120CC4, D_80120CD0);

    D_800EAFE8 = 8;
    D_800EAFA4 = 8;
    D_800EB028 = D_800EAFA0;
    D_800EB03E = 4;
    func_80011DFC();
}
