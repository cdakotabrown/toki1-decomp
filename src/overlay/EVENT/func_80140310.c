/* Address: 0x80140310 */
/* Size: 204 bytes (51 instructions) -- TARGET: 51, COMPILED: 44 */
#include "types.h"

extern s32 D_80122B30;
extern s32 D_80122B34;
extern s32 D_80122B38;
extern s16 D_80122B3C;
extern s32 D_80122B40;
extern s32 D_80122B44;
extern s32 D_80122B48;
extern s32 D_80122B4C;
extern s32 D_80122B50;

extern s32 D_80122B54;
extern s32 D_80122B58;

extern void func_80103C80(void);
extern void func_80012D64();
extern void func_8004C46C();
extern void func_8004C6B0();
extern void func_80011DFC();

void func_80140310(void) {
    func_80103C80();
    func_80012D64(D_80122B40, 17, 1, 2, 0);
    func_8004C46C(D_80122B44, D_80122B48, D_80122B4C, D_80122B50, D_80122B54, D_80122B58);
    func_8004C6B0(D_80122B34, D_80122B38, D_80122B30, D_80122B3C);
    func_80011DFC();
}
