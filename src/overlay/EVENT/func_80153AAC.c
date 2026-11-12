/* Address: 0x80153AAC, Size: 132 bytes (33 instructions) */
/* @SymbolStart: 0x80153A68 */
/* @SymbolSize: 200 */
/* @PostProc: patch_insn_reg_field 0x94:rt:t3:t4 0x98:rs:t3:t4 */
#include "types.h"

extern s16 D_80124344;
extern s16 D_80124348;
extern s32 D_8012425C;
extern s32 D_801242C8;
extern s32 D_80124334;

extern s32 D_8009471C;
extern s32 D_80094720;
extern s32 D_80094724;
extern s32 D_80094728;
extern s32 D_8009472C;

extern u8 D_800B1746;
extern s32 D_800EECBC;
extern u8 D_801252E4;

extern s32 func_8002328C();
extern void func_8004A8EC();
extern void func_80033E88();
extern void func_80049B20();

void func_80153AAC(void) {
    s32 result;

    D_8009471C = (s32)&D_80124344;
    D_80094720 = (s32)&D_80124348;
    D_80094724 = D_8012425C;
    D_80094728 = D_801242C8;
    D_8009472C = D_80124334;

    result = func_8002328C(D_800B1746);

    if ((s32)D_801252E4 - 2 != 0) {
        goto skip;
    }
    if ((s32)D_80124348 - 2 != 0) {
        goto skip;
    }
    if ((u32)(result & 0x7F) < 2U) {
        goto skip;
    }

    func_8004A8EC(3);
    func_80033E88();

skip:
    func_80049B20(D_800EECBC, 1, 0);
}
