/* Address: 0x8015465C (true start 0x80154618: the head copies are hoisted */
/* Size: 0x80154690 - 0x80154618 = 120 bytes (30 instructions) */
/* @SymbolStart: 0x80154618 */
/* @SymbolSize: 120 */
#include "types.h"

extern u8 D_80124384;
extern u8 D_80124388;
extern s32 D_801241D4;
extern s32 D_801241E4;
extern s32 D_801241F4;
extern s32 D_8009471C;
extern s32 D_80094720;
extern s32 D_80094724;
extern s32 D_80094728;
extern s32 D_8009472C;

extern void func_80049B20(s32, s32, s32);

void func_8015465C(void) {
    D_8009471C = (s32)&D_80124384;
    D_80094720 = (s32)&D_80124388;
    D_80094724 = D_801241D4;
    D_80094728 = D_801241E4;
    D_8009472C = D_801241F4;
    func_80049B20(255, 1, 0);
}
