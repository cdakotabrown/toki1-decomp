/* @CompileEnv: IDO52_AS1_NO_LA_PULL=6,7 */
/* func_8013A774 -- GYOZI overlay, Address: 0x8013A774, Size: 368 bytes (92 insns) */
/* @PostProc: patch_stack_complete --old 184 --new 200 */
/* @PostProc: remap_stack_slots --map '40:52' */
/* @PostProc: scheduler_normalizer */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr t[36];
} Table36;

extern Table36 D_80147C6C;
extern u8 D_800F647A;
extern volatile s32 D_800F62F0;
extern u8 D_800F53DF;
extern void *D_80147AE0;
extern void *D_80147AE4;
extern char D_80145BB0[];
extern char D_80145BB4[];

extern void func_8004EE18(void *a0, s32 a1, s32 a2, s32 a3, s32 e);
extern void func_800BCDF0(char *a0);

void func_8013A774(void)
{
    Table36 tbl;
    u32 girl;

    tbl = D_80147C6C;
    tbl.t[D_800F647A]();

    if (D_800F62F0 & 0x100) {
        girl = D_800F53DF;
        if (girl >= 6U && girl < 9U) {
            func_8004EE18(D_80147AE0, 17, 1, 0, 0);
        } else {
            func_8004EE18(D_80147AE4, 17, 1, 0, 0);
        }
        func_800BCDF0(D_80145BB0);
    }
    if (D_800F62F0 & 0x800) {
        girl = D_800F53DF;
        if (girl >= 6U && girl < 9U) {
            func_8004EE18(D_80147AE0, 17, 1, 2, 0);
        } else {
            func_8004EE18(D_80147AE4, 17, 1, 2, 0);
        }
        func_800BCDF0(D_80145BB4);
    }
}
