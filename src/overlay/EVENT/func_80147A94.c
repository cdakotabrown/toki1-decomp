/* Address: 0x80147A94 */
/* Size: 168 bytes (42 instructions) */
#include "types.h"

extern s32 D_801240F0;
extern s32 D_801240F4;
extern s32 D_801240F8;
extern s16 D_801240FC;
extern s32 D_80124100;
extern s32 D_80124104;
extern s32 D_80124108;
extern s32 D_8012410C;
extern s32 D_80124110;

extern s32 D_80124114;
extern s32 D_80124118;

extern void func_8010B360(void);
extern void func_80012D64();
extern void func_8004C46C();
extern void func_8004C6B0();
extern void func_80011DFC();

void func_80147A94(void) {
    func_8010B360();
    func_80012D64(D_80124100, 17, 1, 2, 0);
    func_8004C46C(D_80124104, D_80124108, D_8012410C, D_80124110, D_80124114, D_80124118);
    func_8004C6B0(D_801240F4, D_801240F8, D_801240F0, D_801240FC);
    func_80011DFC();
}
