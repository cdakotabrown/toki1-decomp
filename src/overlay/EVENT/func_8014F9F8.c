/* func_8014F9F8 -- EVENT overlay, Address: 0x8014F9F8 */
/* @SymbolStart: 0x8014F9CC */
/* @SymbolSize: 116 */
#include "types.h"

extern s32 D_80124238;
extern s32 D_801242A4;
extern s32 D_80124310;
extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;
extern u8 D_801252E8;
extern u8 D_80094784[];
extern u8 D_8011FDD0[];

extern void func_80078970(u32, u32);
extern void func_800469F4(u32);
extern void func_80011DFC(void);

void func_8014F9F8(void) {
    D_8009473C = D_80124238;
    D_80094740 = D_801242A4;
    D_80094744 = D_80124310;
    D_801252E8 = 0;
    func_80078970((u32)D_80094784, (u32)D_8011FDD0);
    func_800469F4(17548);
    func_80011DFC();
}
