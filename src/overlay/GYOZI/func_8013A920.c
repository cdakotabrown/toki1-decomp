/* @SymbolStart: 0x8013A91C */
/* @SymbolSize: 180 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x68:rt:t8:a0 0x74:rt:t8:a0 0x80:rt:t1:t8 0x84:rs:t1:t8 0x84:rt:t1:t8 0x94:rt:t1:t8 */
#include "types.h"

extern void func_8009068C(void);
extern void func_80089200(void);
extern void func_8008FC10(void);
extern void func_8008F618(void);
extern void func_8013A140(void);
extern void func_8004DE1C(void);

extern s8 D_800C51C4;
extern u8 D_800F53DA;
extern u8 D_800F62CF;
extern s8 D_800F6412;
extern s32 D_800F6458;
extern s32 D_8012E684;
extern s32 D_8012E688;
extern s8 D_801317EB;
extern s32 D_80147440;
extern s32 D_80147464;
extern s32 D_80147488;
extern s32 D_8014749C;
extern s32 D_801474A0;
extern s32 D_801474A4;

void func_8013A920(void)
{
    D_800F6412 = 0;
    D_800F6458 = 1;
    func_8009068C();
    D_800F53DA = 128;
    D_801317EB = 0;
    D_8012E688 = 0;
    D_8012E684 = 0;
    D_800C51C4 = 0;
    func_80089200();
    func_8008FC10();
    D_800F62CF = 1;
    func_8008F618();
    func_8013A140();
    D_801474A0 = D_80147464;
    D_801474A4 = D_80147488;
    D_8014749C = D_80147440;
    func_8004DE1C();
}
