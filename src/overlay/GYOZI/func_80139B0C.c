/* @SymbolSize: 240 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0xa4:rt:t8:a0 0xb0:rt:t8:a0 0xbc:rt:t1:t8 0xc0:rs:t1:t8 0xc0:rt:t1:t8 0xd0:rt:t1:t8 */
/* func_80139B0C -- GYOZI overlay, Address: 0x80139B0C, Size: 240 bytes (60 insns) */
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
extern s32 D_80147438;
extern s32 D_8014745C;
extern s32 D_80147480;
extern s32 D_8014749C;
extern s32 D_801474A0;
extern s32 D_801474A4;

void func_80139B0C(void)
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
    D_800F62CF = 3;
    func_8008F618();
    func_8013A140();
    D_801474A0 = D_8014745C;
    D_801474A4 = D_80147480;
    D_8014749C = D_80147438;
    func_8004DE1C();
}
