/* @SymbolStart: 0x80136A10 */
/* @SymbolSize: 320 */
/* @PostProc: patch_insn_reg_field 0xa8:rt:t0:at 0xb0:rs:t0:at 0xd4:rt:t1:t0 0xd8:rs:t1:t0 0xd8:rt:t1:t0 0xe0:rt:t2:t1 0xe4:rs:t2:t1 0xe4:rt:t2:t1 0xe8:rt:t1:t0 0xf0:rt:t3:t2 0xf4:rs:t3:t2 0xf4:rt:t3:t2 0xf8:rt:t2:t1 0x100:rt:t3:t2 0x118:rt:t4:t3 0x11c:rt:t4:t3 0x128:rt:t5:t4 0x12c:rs:t5:t4 0x12c:rt:t5:t4 0x134:rs:t5:t4 0x134:rt:t6:t5 0x13c:rt:t6:t5 */
/* Address: 0x80136A10, Size: 320 bytes (80 insns) */
#include "types.h"

extern u8 D_800F5AAE;
extern s32 D_800D92A0;
extern s32 D_801459E0;
extern u8 D_800F6412;
extern s32 D_800F6458;
extern u8 D_800F53DA;
extern u8 D_801317EB;
extern s32 D_80147100;
extern s32 D_80147134;
extern s32 D_80147168;
extern s32 D_8014718C;
extern s32 D_80147190;
extern s32 D_80147194;
extern s16 D_80147198;
extern s16 D_8014719C;
extern u8 D_800F62CF;

extern void func_8004EDE0(s32, s32);
extern void func_80054864(s32);
extern void func_8004C670(void);
extern void func_8005493C(s32);
extern void func_8004EDF4(s32);
extern void func_80053EFC(void);
extern void func_8005ABC0(void);
extern void func_800BCE10(s32 *, s32 *);
extern void func_8009068C(void);
extern void func_80089200(void);
extern void func_800744A0(void);
extern void func_80074950(void);
extern void func_8008FC10(void);
extern void func_80138510(void);
extern void func_8008F618(s32);
extern void func_8004DE1C(void);

void func_80136A18(void)
{
    u32 hidden_t6 = D_800F5AAE;

    D_800F5AAE = (u8)(hidden_t6 | 0x10);
    func_8004EDE0(1, 0);
    func_80054864(1);
    func_8004C670();
    func_8005493C(0);
    func_8004EDF4(1);
    func_80053EFC();
    func_8005ABC0();
    func_800BCE10(&D_800D92A0, &D_801459E0);
    D_800F6412 = 0;
    D_800F6458 = 1;
    func_8009068C();
    D_800F53DA = 128;
    D_801317EB = 0;
    *(u8 *)0x800C51C4 = 0;
    func_80089200();
    func_800744A0();
    func_80074950();
    func_8008FC10();
    func_80138510();
    D_8014718C = D_80147100;
    D_80147190 = D_80147134;
    D_80147194 = D_80147168;
    D_80147198 = 0;
    D_8014719C = 0;
    D_800F62CF = 12;
    func_8008F618(12);
    D_800F5AAE = (u8)(D_800F5AAE | 0x08);
    func_8004DE1C();
}
