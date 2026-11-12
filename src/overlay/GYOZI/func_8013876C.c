/* func_8013876C -- GYOZI overlay, Address: 0x8013876C, Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 func_80050AB8(void);
extern void func_8004DDD8(void);

void func_8013876C(void)
{
    if (func_80050AB8() != 1) {
        return;
    }
    func_8004DDD8();
}
