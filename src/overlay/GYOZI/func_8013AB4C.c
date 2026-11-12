/* func_8013AB4C -- GYOZI overlay, Address: 0x8013AB4C, Size: 400 bytes */
/* @SymbolStart: 0x8013ab18 */
/* @SymbolSize: 452 */
/* @PostProc: patch_insn_reg_field 0x30:rt:t9:t0 0x34:rt:t1:t2 0x38:rs:t9:t0 0x38:rt:t0:t1 0x3c:rt:t0:t1 0x48:rt:t1:t2 0xb0:rt:t2:t3 0xb4:rt:t3:t4 0xb8:rs:t3:t4 0xb8:rt:t3:t4 0xbc:rs:t2:t3 0xbc:rt:t2:t3 0xe0:rt:t3:t4 0xe8:rt:t2:t3 0x130:rt:t4:t5 0x134:rt:t5:t6 0x138:rs:t5:t6 0x138:rt:t5:t6 0x13c:rs:t4:t5 0x13c:rt:t4:t5 0x160:rt:t5:t6 0x168:rt:t4:t5 0x194:rt:t6:t7 0x198:rs:t6:t7 0x198:rt:t6:t7 0x1a0:rd:t7:t8 0x1a0:rs:t6:t7 0x1a8:rt:t7:t8 0x1b8:rt:t8:t9 0x1c0:rt:t8:t9 */
#include "types.h"

extern void func_800BCE10(void *a0, void *a1);
extern void func_8013C3B0(void);
extern void func_8004EE18(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void func_8008FD1C(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void func_8008FF60(s32 a0, s32 a1, s32 a2, s32 a3);
extern void func_8004DE1C(void);

extern char D_800D92A0[];
extern u8 D_800F53A0[];
extern u8 D_800F53DF;
extern u8 D_800F62CF;
extern s16 D_8012B8D6;
extern s32 D_8012B8F8;
extern s32 D_8012E69C;
extern char D_80145BD8[];
extern s16 D_801474A8;
extern s16 D_801474AC;
extern s32 D_80147AD0;
extern s32 D_80147AD4;
extern s32 D_80147AD8;
extern s16 D_80147ADC;
extern s32 D_80147AE0;
extern s32 D_80147AE4;
extern s32 D_80147AE8;
extern s32 D_80147AEC;
extern s32 D_80147AF0;
extern s32 D_80147AF4;
extern s32 D_80147AF8;
extern s32 D_80147AFC;
extern s32 D_80147B00;
extern s32 D_80147B04;
extern s32 D_80147B08;
extern s32 D_80147B0C;
extern s32 D_80147B10;
extern s32 D_80147B14;

void func_8013AB4C(void)
{
    D_800F53A0[D_800F62CF * 56 + 440] |= 4;
    D_801474A8 = 13;
    D_801474AC = 0;
    func_800BCE10(D_800D92A0, D_80145BD8);
    func_8013C3B0();
    if ((u32)D_800F53DF >= (u32)6 && (u32)D_800F53DF < (u32)10) {
        func_8004EE18(D_80147AE0, 17, 1, 2, 0);
        func_8008FD1C(D_80147AE8, D_80147AEC, D_80147AF0, D_80147AF4,
                      D_80147AF8, D_80147AFC);
        func_8008FF60(D_80147AD4, D_80147AD8, D_80147AD0, D_80147ADC);
    } else {
        func_8004EE18(D_80147AE4, 17, 1, 2, 0);
        func_8008FD1C(D_80147B00, D_80147B04, D_80147B08, D_80147B0C,
                      D_80147B10, D_80147B14);
        func_8008FF60(D_80147AD4, D_80147AD8, D_80147AD0, D_80147ADC);
    }
    D_8012B8F8 |= 0x80000000;
    D_8012B8D6 = 0;
    D_8012E69C = 1;
    func_8004DE1C();
}
