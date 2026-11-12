/* @SymbolSize: 176 */
/* @PostProc: patch_stack_complete --old 40 --new 48 */
/* @PostProc: remap_stack_slots --from 31 --to 43 */
/* func_80141E08 -- GYOZI overlay, Address: 0x80141E08, Size: 176 bytes (44 insns) */
#include "types.h"

extern s32 func_8005B368(void);
extern void func_80140050(void);

extern s32 D_800F6474;
extern u8 D_8012B8C2;
extern s32 D_8012E668;

void func_80141E08(void)
{
    s32 v0;
    s32 x;
    u8 stackByte;

    stackByte = 1;
    v0 = func_8005B368();

    if ((u32)v0 >= 2) {
        D_800F6474 = D_800F6474 + 1;
    }

    x = D_800F6474;
    if ((u32)x >= 115 && (u32)x % 60 == 6) {
        goto do_call;
    }
    if (D_8012B8C2 & 1) {
        goto do_call;
    }
    stackByte = 0;

do_call:
    func_80140050();

    if (stackByte == 0 && D_8012E668 != 0) {
        D_8012B8C2 = 0;
    }
}
