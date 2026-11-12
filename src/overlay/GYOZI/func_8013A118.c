/* @CompileEnv: IDO52_CONST_INREG_VALUES=7 */
/* @SymbolSize: 504 */
/* func_8013A118 -- GYOZI overlay, Address: 0x8013A118, Size: 504 bytes (126 insns) */
/* `@CompileEnv: IDO52_CONST_INREG_VALUES=7` (Hook 16, UNWINDOWED) forces the */
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
extern void func_8008F618(s32 a0);
extern void func_8013A140(void);
extern void func_8013BEA0(void);
extern void func_8004EE18(s32 a0, s32 a1, s32 a2, s32 a3, s32 s4);
extern void func_8008FD1C(s32 a0, s32 a1, s32 a2, s32 a3, s32 s4, s32 s5);
extern void func_8008FF60(s32 a0, s32 a1, s32 a2, s32 a3);
extern void func_8004DE1C(void);

extern s8 D_800C51C4;
extern u8 D_800F53DA;
extern u8 D_800F53DF;
extern s8 D_800F5ACD;
extern s8 D_800F62CF;
extern s8 D_800F6412;
extern s32 D_800F6458;
extern s8 D_801317EB;
extern s32 D_8014743C;
extern s32 D_80147460;
extern s32 D_80147484;
extern s32 D_8014749C;
extern s32 D_801474A0;
extern s32 D_801474A4;
extern s32 D_801479B0;
extern s32 D_801479B4;
extern s32 D_801479B8;
extern s16 D_801479BC;
extern s32 D_801479C0;
extern s32 D_801479C4;
extern s32 D_801479C8;
extern s32 D_801479CC;
extern s32 D_801479D0;
extern s32 D_801479D4;
extern s32 D_801479D8;
extern s32 D_801479DC;
extern s32 D_801479E0;
extern s32 D_801479E4;
extern s32 D_801479E8;
extern s32 D_801479EC;
extern s32 D_801479F0;
extern s32 D_801479F4;

void func_8013A118(void)
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
    D_800F5ACD = 7;
    D_800F62CF = 7;
    func_8008F618(7);
    func_8013A140();
    D_8014749C = D_8014743C;
    D_801474A0 = D_80147460;
    D_801474A4 = D_80147484;
    func_8013BEA0();
    if ((u32)D_800F53DF >= 6 && (u32)D_800F53DF < 10) {
        func_8004EE18(D_801479C0, 17, 1, 2, 0);
        func_8008FD1C(D_801479C8, D_801479CC, D_801479D0, D_801479D4,
                      D_801479D8, D_801479DC);
    } else {
        func_8004EE18(D_801479C4, 17, 1, 2, 0);
        func_8008FD1C(D_801479E0, D_801479E4, D_801479E8, D_801479EC,
                      D_801479F0, D_801479F4);
    }
    func_8008FF60(D_801479B4, D_801479B8, D_801479B0, D_801479BC);
    func_8004DE1C();
}
