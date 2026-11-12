/* Address: 0x80139A9C */
/* Size: 148 bytes (37 instructions) */
#include "types.h"

extern s32 D_801215E0;
extern s32 D_801215E4;
extern s32 D_801215E8;
extern s32 D_801215EC;
extern s32 D_801215F0;
extern s32 D_801215F4;
extern s32 D_801215F8;

extern void func_80011DFC(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s32);
extern void func_800FD2F0(void);

void func_80139A9C(void)
{
    func_800FD2F0();
    func_80012D64(D_801215E0, 17, 1, 2, 0);
    func_8004C46C(D_801215E4, D_801215E8, D_801215EC, D_801215F0, D_801215F4, D_801215F8);
    func_8004C6B0(0, 0, 0, 0);
    func_80011DFC();
}
