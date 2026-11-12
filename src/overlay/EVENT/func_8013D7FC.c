/* Address: 0x8013D7FC */
/* Size: 168 bytes (42 instructions) */
#include "types.h"

extern s32 D_801221D0;
extern s32 D_801221D4;
extern s32 D_801221D8;
extern s16 D_801221DC;
extern s32 D_801221E0;
extern s32 D_801221E4;
extern s32 D_801221E8;
extern s32 D_801221EC;
extern s32 D_801221F0;
extern s32 D_801221F4;
extern s32 D_801221F8;

extern void func_801011A0(void);
extern void func_80012D64();
extern void func_8004C46C();
extern void func_8004C6B0();
extern void func_80011DFC();

void func_8013D7FC(void) {
    func_801011A0();
    func_80012D64(D_801221E0, 17, 1, 2, 0);
    func_8004C46C(D_801221E4, D_801221E8, D_801221EC, D_801221F0, D_801221F4, D_801221F8);
    func_8004C6B0(D_801221D4, D_801221D8, D_801221D0, D_801221DC);
    func_80011DFC();
}
