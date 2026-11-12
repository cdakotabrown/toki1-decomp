/* func_801330B8 -- EVENT overlay, Address: 0x801330B8 */
/* @SymbolStart: 0x80133074 */
/* @SymbolSize: 124 */
#include "types.h"

extern u8 D_80094714[];
extern u8 D_80094718[];
extern u32 D_8009471C;
extern u32 D_80094720;
extern s32 D_80094724;
extern s32 D_80094728;
extern s32 D_8009472C;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_800EECBC;

extern void func_80049B20(s32, s32, s32);

void func_801330B8(void) {
    D_8009471C = (u32)D_80094714;
    D_80094720 = (u32)D_80094718;
    D_80094724 = D_80120678;
    D_80094728 = D_8012067C;
    D_8009472C = D_80120680;
    func_80049B20(D_800EECBC, 1, 0);
}
