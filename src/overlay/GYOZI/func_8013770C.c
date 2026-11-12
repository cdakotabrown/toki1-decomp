/* func_8013770C -- GYOZI overlay, Address: 0x8013770C, Size: 88 bytes (22 insns) */
#include "types.h"

extern s32 func_800BDC20(void *src, void *dst);
extern void func_8004DE1C(void);
extern void func_800BCE10(void *a0, void *a1);

extern u8 D_800D92A0;
extern u8 D_800D92E0;

void func_8013770C(void)
{
    if (func_800BDC20(&D_800D92A0, &D_800D92E0) == 0) {
        func_8004DE1C();
    }
    func_800BCE10(&D_800D92A0, &D_800D92E0);
    func_8004DE1C();
}
