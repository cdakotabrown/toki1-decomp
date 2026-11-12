/* @CompileEnv: IDO52_AS1_PASS1_VOLATILE_OK=all IDO52_AS1_XBB_VOLATILE_OK=0 IDO52_UGEN_TEMP_REG=15:t1,16:t2,17:t3,18:t4,19:t5,20:t6,21:t7,22:t8 */
/* @SymbolStart: 0x8013E464 */
/* @SymbolSize: 576 */
/* func_8013E478 -- GYOZI overlay, Address: 0x8013E478, Size: 556 bytes */
#include "types.h"

extern void func_8004DE48(s32);
extern void func_8004EDE0(s32, s32);
extern void func_80054864(s32);
extern void func_8004C670(void);
extern void func_8005493C(s32);
extern void func_8004EDF4(s32);
extern void func_80053EFC(void);
extern void func_8005ABC0(void);
extern void func_8009068C(void);
extern void func_800BCE10(void *, void *);
extern void func_80089200(void);
extern void func_800744A0(void);
extern void func_80074950(void);
extern void func_8008FC10(void);
extern void func_8013EB10(void);
extern s32 func_8004DE1C(void);

extern u8 D_800C51C4;
extern u8 D_800D9264[];
extern u8 D_800D9280[];
extern u8 D_800D92A0[];
extern u8 D_800F53DA;
extern u32 D_800F5490;
extern u32 D_800F555C;
extern u8 D_800F594E;
extern u8 D_800F5A2D[];
extern volatile u8 D_800F5AAC;
extern volatile u8 D_800F5AAE;
extern u8 D_800F62CF;
extern u8 D_800F6412;
extern s32 D_800F6458;
extern u8 D_801317EB;
extern u8 D_80145CF8[];
extern u8 D_80145D04[];
extern u8 D_80145D0C[];
extern u8 D_80145D14[];
extern s32 D_801481FC;
extern s32 D_80148230;
extern s32 D_80148264;
extern s32 D_80148614;
extern s32 D_80148618;
extern s32 D_8014861C;
extern s16 D_80148620;
extern s16 D_80148624;

s32 func_8013E478(void)
{
    u32 a;

    a = (u8)(D_800F5AAE | 0x20);
    if ((D_800F555C & 0xF) == 12) {
        if (((D_800F555C << 23) >> 27) == 24) {
            if (a != 0) {
                D_800F5AAE = (u8)a;
                D_800F5AAE |= 1;
                D_800F5A2D[D_800F5AAC * 4] = 0;
                func_8004DE48(87);
                return 0;
            }
        }
    }

    a = (u8)(a | 0x40);
    D_800F5AAE = (u8)a;
    if ((D_800F5490 & 0xF) == 12) {
        D_800F5AAE = (u8)a;
        if (((D_800F5490 << 23) >> 27) == 24) {
            D_800F5AAE = (u8)a;
            if (a != 0) {
                D_800F5AAE = (u8)(a | 4);
                func_8004DE48(86);
                return 0;
            }
        }
    }

    func_8004EDE0(1, 0);
    func_80054864(1);
    func_8004C670();
    func_8005493C(0);
    func_8004EDF4(1);
    func_80053EFC();
    func_8005ABC0();
    func_8009068C();
    func_800BCE10(D_800D92A0, D_80145CF8);
    D_800F6412 = 0;
    D_800F6458 = 1;
    D_800F53DA = 128;
    D_801317EB = 0;
    D_800C51C4 = 0;
    D_80148620 = 0;
    D_80148624 = 0;
    func_80089200();
    func_800744A0();
    func_80074950();
    func_8008FC10();
    func_800BCE10(D_800D9264, D_80145D04);
    if (D_800F594E == 0) {
        func_800BCE10(D_800D9280, D_80145D0C);
    } else {
        func_800BCE10(D_800D9280, D_80145D14);
    }
    D_800F62CF = 12;
    func_8013EB10();
    D_80148614 = D_801481FC;
    D_80148618 = D_80148230;
    D_8014861C = D_80148264;
    return func_8004DE1C();
}
