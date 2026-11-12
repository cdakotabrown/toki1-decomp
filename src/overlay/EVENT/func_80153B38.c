/* @SymbolStart: 0x80153B30 */
/* @SymbolSize: 444 */
/* @PostProc: patch_insn_reg_field 0x44:rt:a0:v1 0x48:rs:a0:v1 0x48:rt:a0:v1 0x50:rs:a0:v1 0x60:rt:v1:a0 0x64:rs:v1:a0 0x64:rt:v1:a0 0x68:rt:a0:v1 0x6c:rs:a0:v1 0x6c:rt:a0:v1 0x74:rs:v1:a0 0x7c:rs:v0:a1 0x7c:rt:a1:v0 0x84:rs:a0:v1 0x9c:rs:v1:a0 0xa4:rs:v0:a1 0xa4:rt:a1:v0 0xac:rs:a0:v1 0xc4:rs:v1:a0 0xcc:rs:v0:a1 0xcc:rt:a1:v0 0xd4:rs:a0:v1 0xf4:rs:a0:v1 0x134:rs:a0:v1 */
/* Address: 0x80153B38 */
/* Size: 436 bytes (109 instructions) */
#include "types.h"

extern s32 D_800B1AF0;
extern u8 D_800B0895;
extern u8 D_800B0896;
extern u8 D_800B0897;
extern s32 D_800B1AE4;
extern s16 D_8012434C;
extern s32 D_80124350;
extern s32 D_80124260;
extern s32 D_801242CC;
extern s32 D_80124338;
extern s32 D_8009471C;
extern s32 D_80094720;
extern s32 D_80094724;
extern s32 D_80094728;
extern s32 D_8009472C;
extern s32 D_800EECBC;

extern s32 func_80049B20(s32, s32, s32);
extern void func_80011DFC(void);

s32 func_80153B38(void) {
    u32 once;
    s32 first;
    s32 b895, b896, b897;

    once = D_800B1AF0;
    first = (once < 1);
    D_800B1AF0 = once + 1;
    if (first) {
        b896 = D_800B0896;
        if (b896 == 3 && D_800B0897 == 21) {
            D_8012434C = 0;
            goto publish;
        }
        b895 = D_800B0895;
        b897 = D_800B0897;
        if (b895 == 95 && b896 == 7 && b897 == 24) {
            D_8012434C = 1;
            goto publish;
        }
        if (b895 == 96 && b896 == 7 && b897 == 28) {
            D_8012434C = 1;
            goto publish;
        }
        if (b895 == 97 && b896 == 7 && b897 == 27) {
            D_8012434C = 1;
            goto publish;
        }
        if (b896 == 9 && b897 == 23) {
            if (D_800B1AE4 & 1) {
                D_8012434C = 2;
            } else {
                D_8012434C = 3;
            }
            goto publish;
        }
        if (b896 == 12 && b897 == 23) {
            D_8012434C = 4;
            goto publish;
        }
        func_80011DFC();
        return 0;
    }
publish:
    D_8009471C = (s32)&D_8012434C;
    D_80094720 = (s32)&D_80124350;
    D_80094724 = D_80124260;
    D_80094728 = D_801242CC;
    D_8009472C = D_80124338;
    return func_80049B20(D_800EECBC, 1, 0);
}
