/* Address: 0x8013D8FC */
/* Size: 168 bytes (42 instructions) */
#include "types.h"

extern s32 D_801221FC;
extern s32 D_80122200;
extern s32 D_80122204;
extern s16 D_80122208;
extern s32 D_8012220C;
extern s32 D_80122210;
extern s32 D_80122214;
extern s32 D_80122218;
extern s32 D_8012221C;

extern s32 D_80122220;
extern s32 D_80122224;

extern void func_80101250(void);
extern void func_80012D64();
extern void func_8004C46C();
extern void func_8004C6B0();
extern void func_80011DFC();

void func_8013D8FC(void) {
    func_80101250();
    func_80012D64(D_8012220C, 17, 1, 2, 0);
    func_8004C46C(D_80122210, D_80122214, D_80122218, D_8012221C, D_80122220, D_80122224);
    func_8004C6B0(D_80122200, D_80122204, D_801221FC, D_80122208);
    func_80011DFC();
}
