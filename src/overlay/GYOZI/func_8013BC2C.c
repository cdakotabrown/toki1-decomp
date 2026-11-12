/* @SymbolSize: 268 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=10 */
/* @PostProc: patch_insn_reg_field 0x6c:rt:t7:t8 0x74:rs:t7:t8 0x74:rt:t8:t9 0x7c:rt:t8:t9 0x80:rt:t9:t0 0x88:rt:t9:t0 0xb8:rt:v0:a0 0xc0:rt:v0:a0 0xcc:rt:v0:a0 0xd8:rt:t0:t1 0xdc:rs:t0:t1 0xdc:rt:t0:t1 0xe4:rt:t1:t2 0xe8:rs:t1:t2 0xe8:rt:t1:t2 0xec:rt:t0:t1 0xf4:rt:t2:t3 0xf8:rs:t2:t3 0xf8:rt:t2:t3 0xfc:rt:t1:t2 0x108:rt:t2:t3 */
/* func_8013BC2C -- GYOZI overlay, Address: 0x8013BC2C, Size: 268 bytes (67 insns) */
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
extern volatile u8 D_800F5788;
extern s8 D_800F5ACD;
extern s8 D_800F62CF;
extern s8 D_800F6412;
extern volatile s32 D_800F6458;
extern s8 D_801317EB;
extern s32 D_80147450;
extern s32 D_80147474;
extern s32 D_80147498;
extern s32 D_8014749C;
extern s32 D_801474A0;
extern s32 D_801474A4;

void func_8013BC2C(void)
{
    s32 ten;

    func_8004EDE0(1, 0);
    func_80054864(1);
    func_8004C670();
    func_8005493C(0);
    func_8004EDF4(1);
    func_80053EFC();
    func_8005ABC0();
    D_800F6412 = 0;
    D_800F6458 = 1;
    D_800F5788 |= 8;
    func_8009068C();
    D_800F53DA = 128;
    D_801317EB = 0;
    D_800C51C4 = 0;
    func_80089200();
    func_800744A0();
    func_80074950();
    func_8008FC10();
    ten = 10;
    D_800F5ACD = ten;
    D_800F62CF = ten;
    func_8008F618();
    func_8013A140();
    D_8014749C = D_80147450;
    D_801474A0 = D_80147474;
    D_801474A4 = D_80147498;
    func_8004DE1C();
}
