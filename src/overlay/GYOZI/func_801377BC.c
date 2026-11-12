/* func_801377BC -- GYOZI overlay, Address: 0x801377BC, Size: 60 bytes (15 insns) */
#include "types.h"

extern void func_800BCE10(void *a0, void *a1);
extern void func_8008A0D4(s32 a0);
extern void func_8004DE1C(void);

extern s32 D_800D92E0;
extern s32 D_80145A40;

void func_801377BC(void)
{
    func_800BCE10(&D_800D92E0, &D_80145A40);
    func_8008A0D4(16410);
    func_8004DE1C();
}
