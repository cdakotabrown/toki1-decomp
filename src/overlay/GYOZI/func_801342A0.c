/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x60 */
/* @PostProc: patch_insn_reg_field 0x18:rt:a1:v1 0x28:rs:a1:v1 0x58:rt:v0:a1 0x64:rs:v0:a1 0x64:rt:v0:a1 0x6c:rt:v0:a1 0x70:rd:v1:v0 0x70:rs:a1:v1 0x74:rs:v1:v0 0x88:rs:v1:v0 0x8c:rt:v0:a1 0x90:rs:v0:a1 0x90:rt:v0:a1 0x98:rt:v0:a1 0x9c:rs:a1:v1 */
/* Address: 0x801342A0, Size: 284 bytes (71 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr e[30];
} Tbl;

extern Tbl D_801461A4;
extern u8 D_80145F60;
extern volatile u8 D_800F647A;
extern u8 D_801461A0;
extern void func_80089BD4(s32);
extern void func_80089B6C(s32);
extern void func_800904A8(s32, s32, s32, s32, s32, s32, s32);
extern void func_80054FCC(s32);

void func_801342A0(void)
{
    s32 idx;
    Tbl localTable;

    localTable = D_801461A4;
    if (D_80145F60 != 0) {
        idx = D_800F647A;
        if (localTable.e[idx] == (FuncPtr)func_80089BD4) {
            localTable.e[idx] = (FuncPtr)func_80089B6C;
        }
    }
    idx = D_800F647A;
    localTable.e[idx](64);
    if (D_801461A0 != 0) {
        func_800904A8(-160, -120, 320, 100, 13, 0xE0FF, 0xFFFF);
        func_800904A8(-160, -20, 320, 60, 13, 24672, 0xE0FF);
        func_80054FCC(13);
    }
}
