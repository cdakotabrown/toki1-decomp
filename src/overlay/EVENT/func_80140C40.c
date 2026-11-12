/* @SymbolStart: 0x80140C3C */
/* @SymbolSize: 268 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=127 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x74:rt:a1:a0 0x7c:rs:a1:a0 0x7c:rt:a1:a0 0x88:rt:a0:a1 0x8c:rt:a0:a1 0xa4:rs:t3:a0 0xa4:rt:a1:t3 */
#include "types.h"

typedef struct { s32 w[17]; } Blk68;

extern Blk68 D_800EAFA0;
extern Blk68 D_800EB028;
extern u8 D_800EAFA3;
extern u8 D_800EAFE7;
extern s16 D_800EB03E;
extern u8 D_800EB02A;
extern u8 D_800EC190;
extern s32 ev_flag08;
extern u8 D_800B0866;
extern u8 D_800B0867;
extern u8 D_800B0868;

extern void func_80011DFC();

void func_80140C40(void)
{
    u8 *base;
    s16 i;

    D_800EB028 = D_800EAFA0;
    D_800EB03E = 4;
    D_800EB02A = 9;
    base = (u8 *)&D_800EC190;
    for (i = 0; i < 6; i++) {
        *(u32 *)(base + i * 36 + 0x90) |= 0x80000000;
    }
    D_800EAFA3 = (u8)(D_800EAFA3 & 0xFF7F);
    D_800EAFE7 = (u8)(D_800EAFE7 & 0xFF7F);
    ev_flag08 = 0;
    D_800B0867 = 127;
    D_800B0866 = 127;
    D_800B0868 = 127;

    func_80011DFC();
}
