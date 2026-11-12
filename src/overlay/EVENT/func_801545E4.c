/* Address: 0x801545E4 (true start 0x801545A0: the head copies are hoisted */
/* Size: 0x80154618 - 0x801545A0 = 120 bytes (30 instructions) */
/* @SymbolStart: 0x801545A0 */
/* @SymbolSize: 120 */
#include "types.h"

extern u8 D_8012437C;
extern u8 D_80124380;
extern s32 D_80124224;
extern s32 D_80124290;
extern s32 D_801242FC;
extern s32 D_8009471C;
extern s32 D_80094720;
extern s32 D_80094724;
extern s32 D_80094728;
extern s32 D_8009472C;

extern void func_80049B20(s32, s32, s32);

void func_801545E4(void) {
    D_8009471C = (s32)&D_8012437C;
    D_80094720 = (s32)&D_80124380;
    D_80094724 = D_80124224;
    D_80094728 = D_80124290;
    D_8009472C = D_801242FC;
    func_80049B20(255, 1, 0);
}
