/* @SymbolSize: 144 */
/* Address: 0x80135CAC, Size: 144 bytes (36 insns + 2-insn cross-prologue tail) */
#include "types.h"

extern u8 D_800F62CF;
extern s16 D_800D9248;

s32 system_config_disp(s32 arg0);
void func_8004DE1C(void);

void func_80135CAC(void)
{
    s32 v0;
    s32 v1;
    u8 masked;

    v0 = system_config_disp((s32)D_800F62CF);

    v1 = v0;
    masked = (u8)(v1 & 0x7F);
    v1 = masked;
    if ((u32)v1 < 2U) {
        D_800D9248 = 6;
    } else if (v1 == 2) {
        D_800D9248 = 8;
    } else if (v1 == 3) {
        D_800D9248 = 10;
    } else {
        D_800D9248 = 12;
    }
    func_8004DE1C();
}
