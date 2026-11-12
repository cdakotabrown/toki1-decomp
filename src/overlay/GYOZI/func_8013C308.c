/* @SymbolSize: 372 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0xb8:rt:t8:a0 0xc4:rt:t8:a0 0xd0:rt:t1:t8 0xd4:rs:t1:t8 0xd4:rt:t1:t8 0xe4:rt:t1:t8 0x130:rd:t7:t6 0x134:rt:t7:t6 0x138:rt:t6:t5 0x140:rd:t2:t1 0x144:rd:t3:t2 0x144:rt:t2:t1 0x148:rs:t3:t2 0x148:rt:t4:t3 0x150:rs:t4:t3 0x150:rt:t5:t4 0x154:rs:t5:t4 0x15c:rt:t6:t5 */
/* func_8013C308 -- GYOZI overlay, Address: 0x8013C308, Size: 372 bytes (93 insns) */
#include "types.h"

extern void func_8004EDE0(s32 a0, s32 a1);
extern void func_80054864(s32 a0);
extern void func_8004C670(void);
extern void func_8005493C(s32 a0);
extern void func_8004EDF4(s32 a0);
extern void func_80053EFC(void);
extern void func_8005ABC0(void);
extern void func_800BCE10(void *a0, void *a1);
extern void func_8009068C(void);
extern void func_80089200(void);
extern void func_800744A0(void);
extern void func_80074950(void);
extern void func_8008FC10(void);
extern void func_8008F618(void);
extern void func_8013DE30(void);
extern void func_8004DE1C(void);

extern s8 D_800C51C4;
extern u8 D_800F53DA;
extern u8 D_800F62CF;
extern s8 D_800F6412;
extern s32 D_800F6458;
extern s8 D_801317EB;
extern s32 D_8014806C;
extern s32 D_801480A0;
extern s32 D_801480D4;
extern s32 D_801480FC;
extern s32 D_80148100;
extern s32 D_80148104;
extern s16 D_80148108;
extern s16 D_8014810C;
extern s8 D_80148110;

extern s8 D_800D92A0[];

extern s8 D_80145C50[];

extern u8 D_800F53A0[];

void func_8013C308(void)
{
    s16 i;

    func_8004EDE0(1, 0);
    func_80054864(1);
    func_8004C670();
    func_8005493C(0);
    func_8004EDF4(1);
    func_80053EFC();
    func_8005ABC0();
    func_800BCE10(D_800D92A0, D_80145C50);
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
    D_800F62CF = 12;
    func_8008F618();
    func_8013DE30();
    D_80148100 = D_801480A0;
    D_80148104 = D_801480D4;
    D_801480FC = D_8014806C;
    D_80148108 = 0;
    D_8014810C = 0;
    D_80148110 = 0;

    for (i = 0; i < 11; i++) {
        if (D_800F53A0[i * 56 + 441] & 1) {
            D_80148110 = 1;
        }
    }

    func_8004DE1C();
}
