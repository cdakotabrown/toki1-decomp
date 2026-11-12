/* @SymbolSize: 288 */
/* @PostProc: patch_insn_reg_field 0xa4:rt:t8:a0 0xb0:rt:t8:a0 0xbc:rt:t9:t8 0xc0:rs:t9:t8 0xc0:rt:t9:t8 0xc4:rt:t2:t1 0xc8:rs:t2:t1 0xc8:rt:t2:t1 0xd0:rt:t0:t9 0xd4:rs:t0:t9 0xd4:rt:t0:t9 0xd8:rt:t9:t8 0xe0:rt:t1:t0 0xe4:rd:t3:t2 0xe4:rt:t2:t1 0xe8:rs:t1:t0 0xe8:rt:t1:t0 0xec:rt:t4:t3 0xf0:rd:t3:t2 0xf0:rs:t3:t2 0xf0:rt:t2:t1 0xf4:rt:t0:t9 0xfc:rd:t3:t2 0xfc:rt:t3:t2 0x100:rs:t4:t3 0x100:rt:t4:t3 0x104:rs:t3:t2 0x104:rt:t4:t3 0x108:rt:t1:t0 */
/* func_8013B764 -- GYOZI overlay, Address: 0x8013B764, Size: 284 bytes (71 insns) */
#include "types.h"

extern void func_8004EDE0(s32 a0, s32 a1);
extern void func_80054864(s32 a0);
extern void func_8004C670(void);
extern void func_8005493C(s32 a0);
extern void func_8004EDF4(s32 a0);
extern void func_80053EFC(void);
extern void func_8005ABC0(void);
extern void func_8009068C(void);
extern void func_80089200(void);
extern void func_800744A0(void);
extern void func_80074950(void);
extern void func_8008FC10(void);
extern void func_8008F618(void);
extern void func_8013A140(void);
extern void func_8004DE1C(void);

extern s8 D_800C51C4;
extern u8 D_800F53DA;
extern u8 D_800F62CF;
extern s8 D_800F6412;
extern s32 D_800F6458;
extern s8 D_801317EB;
extern u8 D_800F53A0[];
extern s32 D_8014744C;
extern s32 D_80147470;
extern s32 D_80147494;
extern s32 D_8014749C;
extern s32 D_801474A0;
extern s32 D_801474A4;

void func_8013B764(void)
{
    func_8004EDE0(1, 0);
    func_80054864(1);
    func_8004C670();
    func_8005493C(0);
    func_8004EDF4(1);
    func_80053EFC();
    func_8005ABC0();
    D_800F6412 = 0;
    D_800F6458 = 1;
    func_8009068C();
    D_800F53DA = 128;
    D_801317EB = 0;
    D_800C51C4 = 0;
    func_80089200();
    func_800744A0();
    func_80074950();
    func_8008FC10();
    D_800F62CF = 4;
    func_8008F618();
    func_8013A140();
    D_8014749C = D_8014744C;
    D_801474A0 = D_80147470;
    D_801474A4 = D_80147494;
    D_800F53A0[D_800F62CF * 56 + 440] |= 4;
    func_8004DE1C();
}
