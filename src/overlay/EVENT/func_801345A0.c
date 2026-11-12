/* @CompileEnv: IDO52_AS1_NO_LA_PULL=0 IDO52_AS1_NO_FALLTHRU_FILL=1 */
/* @SymbolStart: 0x80134590 */
/* @SymbolSize: 196 */
/* Address: 0x80134590 (absorbed cross-prologue head) .. 0x801345A0 */
/* Size: 196 bytes (49 instructions) */
#include "types.h"

extern u8 D_800EAFA7;
extern u8 D_800EB02F;
extern u8 D_800EB02B;
extern u32 D_800B1AE4;
extern s32 D_801207A8;
extern s16 D_801207AC;

extern void func_80012D64(s32, s32, s32, s32, s32);

void func_801345A0(void) {
    s32 mode;

    mode = D_800EAFA7;
    D_800EB02F = (u8)mode;

    if ((u32)mode < 8) {
        D_800EB02B = D_800EB02B & 0x7F;
    } else {
        D_800EB02B = D_800EB02B | 0x80;
    }

    if (D_800B1AE4 % 13 != 0) {
        return;
    }

    D_801207AC = D_801207AC + 1;
    if (D_801207AC >= 4) {
        D_801207AC = 0;
    }
    func_80012D64(D_801207A8, 18, 1, 3, D_801207AC);
}
