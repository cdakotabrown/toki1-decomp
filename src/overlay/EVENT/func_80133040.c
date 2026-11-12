/* @SymbolStart: 0x80132FFC */
/* @SymbolSize: 120 */
/* func_80133040 -- EVENT overlay, Address: 0x80132FFC (.sym-mislabeled */
#include "types.h"

extern s32 D_80094714;
extern s32 D_80094718;
extern s32 D_8009471C;
extern s32 D_80094720;
extern s32 D_80094724;
extern s32 D_80094728;
extern s32 D_8009472C;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;

extern void func_80049B20(s32, s32, s32);

void func_80133040(void)
{
    D_8009471C = (s32)&D_80094714;
    D_80094720 = (s32)&D_80094718;
    D_80094724 = D_80120678;
    D_80094728 = D_8012067C;
    D_8009472C = D_80120680;
    func_80049B20(255, 1, 0);
}
