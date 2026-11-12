/* Address: 0x8014B4B8 */
/* Size: 164 bytes (41 instructions) */
#include "types.h"

extern void func_8004ACA8(void);
extern void func_8010F55C(void);
extern void func_8010F74C(void);
extern void func_80015FE0(void);
extern void func_80046A7C(void);
extern void func_8004BB54(void);
extern void func_800372E0(s32 arg);
extern void func_800358D0(void);
extern void func_80036A8C(void);
extern void func_80035774(void);
extern void func_8004AEB0(void);

extern u8 D_800B1AF5;

void func_8014B4B8(void)
{
    func_8004ACA8();

    switch (D_800B1AF5) {
    case 0:
        func_8010F55C();
        break;
    case 1:
        func_8010F74C();
        break;
    default:
        func_80015FE0();
        break;
    }

    func_80046A7C();
    func_8004BB54();
    func_800372E0(2);
    func_800358D0();
    func_80036A8C();
    func_80035774();
    func_8004AEB0();
}
