/* @SymbolStart: 0x801407E4 */
/* @SymbolSize: 68 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: patch_insn_reg_field 0x20:rd:v0:t6 0x24:rt:v0:t6 */
/* func_80140800 -- GYOZI overlay, Address: 0x801407E4 (prologue 0x80140800), */
/* Size: 68 bytes (17 insns).  D_8012B920 += 512; once it reaches 4096, */
#include "types.h"

extern void func_8004DE1C(void);

extern s16 D_8012B920;

void func_80140800(void)
{
    s32 v0;

    v0 = (s16)(D_8012B920 + 512);
    D_8012B920 = v0;
    if (v0 >= 4096) {
        func_8004DE1C();
    }
}
