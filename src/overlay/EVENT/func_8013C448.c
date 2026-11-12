/* Address: 0x8013C448 */
/* Size: 168 bytes (42 instructions) */
#include "types.h"

extern s32 D_80121D20;
extern s32 D_80121D24;
extern s32 D_80121D28;
extern s16 D_80121D2C;
extern s32 D_80121D30;
extern s32 D_80121D34;
extern s32 D_80121D38;
extern s32 D_80121D3C;
extern s32 D_80121D40;
extern s32 D_80121D44;
extern s32 D_80121D48;

extern void func_800FFBE0(void);
extern void func_80012D64();
extern void func_8004C46C();
extern void func_8004C6B0();
extern void func_80011DFC();

void func_8013C448(void) {
    func_800FFBE0();
    func_80012D64(D_80121D30, 17, 1, 2, 0);
    func_8004C46C(D_80121D34, D_80121D38, D_80121D3C, D_80121D40, D_80121D44, D_80121D48);
    func_8004C6B0(D_80121D24, D_80121D28, D_80121D20, D_80121D2C);
    func_80011DFC();
}
