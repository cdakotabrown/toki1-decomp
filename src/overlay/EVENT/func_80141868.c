/* Address: 0x80141868 */
/* Size: 168 bytes (42 instructions) */
#include "types.h"

extern s32 D_80122F70;
extern s32 D_80122F74;
extern s32 D_80122F78;
extern s16 D_80122F7C;
extern s32 D_80122F80;
extern s32 D_80122F84;
extern s32 D_80122F88;
extern s32 D_80122F8C;
extern s32 D_80122F90;

extern s32 D_80122F94;
extern s32 D_80122F98;

extern void func_801050D0(void);
extern void func_80012D64();
extern void func_8004C46C();
extern void func_8004C6B0();
extern void func_80011DFC();

void func_80141868(void) {
    func_801050D0();
    func_80012D64(D_80122F80, 17, 1, 2, 0);
    func_8004C46C(D_80122F84, D_80122F88, D_80122F8C, D_80122F90, D_80122F94, D_80122F98);
    func_8004C6B0(D_80122F74, D_80122F78, D_80122F70, D_80122F7C);
    func_80011DFC();
}
