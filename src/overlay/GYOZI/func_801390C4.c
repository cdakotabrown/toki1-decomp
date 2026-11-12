/* func_801390C4 -- GYOZI overlay, Address: 0x801390C4, Size: 268 bytes (67 insns) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr t[36];
} Table36;

extern Table36 D_801476F4;
extern u8 D_800F647A;
extern volatile u8 D_80147650;
extern void func_80056574(s32 a0, s32 a1, s32 a2, s32 a3, s32 e, s32 f);

void func_801390C4(void)
{
    s32 pad_hi;
    Table36 tbl;
    u32 sel;

    tbl = D_801476F4;

    if (D_80147650 == 0) {
        sel = D_800F647A;
        if (sel >= 19U && sel < 28U) {
            func_80056574(96, -16, 48, 8, 8, 13);
        }
    }
    if (D_80147650 == 1) {
        sel = D_800F647A;
        if (sel >= 19U && sel < 28U) {
            func_80056574(-60, -32, 32, 8, 8, 13);
        }
    }
    tbl.t[D_800F647A]();
}
