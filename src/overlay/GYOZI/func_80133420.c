/* @SymbolSize: 88 */
/* /* func_80133420 -- GYOZI overlay, Address: 0x80133420, Size: 88 bytes (22 insns) */
#include "types.h"

extern void func_800BCE10(s32, s32);
extern void func_8004DE1C(void);

extern s32 D_800D92E0;
extern s32 D_801458E8;
extern s8  D_80145EC8;
extern s8  D_80145F60;
extern s16 D_80145F64;
extern s16 D_80145EB4;

void func_80133420(void)
{
    func_800BCE10((s32)&D_800D92E0, (s32)&D_801458E8);
    D_80145F64 = 0;
    D_80145F60 = 0;
    D_80145EC8 = 1;
    D_80145EB4 = 6;
    func_8004DE1C();
}
