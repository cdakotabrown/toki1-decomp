/* func_80143384 -- GYOZI overlay, Address: 0x80143384, Size: 372 bytes (93 insns) */
/* @PostProc: patch_stack_complete --old 216 --new 232 */
/* @PostProc: remap_stack_slots --map '24:40' */
/* @PostProc: swap_instructions --offset1 0x118 --offset2 0x11c */
/* @PostProc: swap_instructions --offset1 0x11c --offset2 0x124 */
/* @PostProc: swap_instructions --offset1 0x120 --offset2 0x124 */
#include "types.h"

typedef s32 (*FuncPtr)(s32);

typedef struct {
    FuncPtr t[47];
} FuncTable47;

typedef struct {
    u8 pad0;
    u8 c7;
    s16 c8;
    u8 pad1[12];
    s16 d6;
    s16 d8;
} GyoziState;

extern FuncTable47 D_8014901C;
extern GyoziState D_8012B8C6;
extern u8 D_800F647A;
extern u8 D_801317EB;
extern s32 D_8012E694;
extern u8 D_8012B94A;
extern u8 D_8012B94B;
extern u8 D_8012B94F;
extern u32 D_800F6468;

extern void func_80145520(void);

s32 func_80143384(void)
{
    s32 sel;
    FuncTable47 localTable;

    localTable = D_8014901C;
    localTable.t[D_800F647A](60);

    sel = D_800F647A;
    if ((u32)sel < 16) {
        D_8012B8C6.c7 = D_801317EB;
        if (D_8012B8C6.d8 == 0 && D_8012B8C6.c8 == 0) {
            D_8012B8C6.d6 = (s16)(D_8012B8C6.d6 + 1);
            if (D_8012B8C6.d6 >= 3) {
                D_8012B8C6.d6 = 0;
            }
        }
    }

    if (D_8012E694 != 0) {
        D_8012B94B |= 0x80;
        D_8012B94F = D_8012B8C6.c7;
        if ((D_800F6468 % 300) == 0) {
            func_80145520();
            D_8012B94A = 3;
        }
    } else {
        D_8012B94B &= 0xFF7F;
    }
}
