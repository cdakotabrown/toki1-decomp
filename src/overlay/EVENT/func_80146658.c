/* Address: 0x80146658 */
/* Size: 140 bytes (35 instructions) */
#include "types.h"

extern s32 D_80123E90;
extern s32 D_80123E94;
extern s32 D_80123E98;
extern s32 D_80123E9C;
extern s32 D_80123EA0;
extern s32 D_80123EA4;
extern s32 D_80123EA8;

extern void func_80012D64();
extern void func_8004C46C();
extern void func_8004C6B0();
extern void func_80011DFC();

void func_80146658(void)
{
    func_80012D64(D_80123E90, 17, 1, 2, 0);
    func_8004C46C(D_80123E94, D_80123E98, D_80123E9C, D_80123EA0,
                  D_80123EA4, D_80123EA8);
    func_8004C6B0(0, 0, 0, 0);
    func_80011DFC();
}
