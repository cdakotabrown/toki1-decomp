/* Address: 0x8014F8D8 (cross-prologue head absorbed) .. 0x8014F910 */
/* Size: 244 bytes (61 instructions) */
/* @SymbolStart: 0x8014F8D8 */
/* @SymbolSize: 244 */
#include "types.h"

extern s32 D_80124250;
extern s32 D_801242BC;
extern s32 D_80124328;
extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;
extern u8 D_800B0895;

extern u8 D_80125304;
extern u8 D_80125308;
extern u8 D_801252E8;
extern u8 D_800B0896;
extern u8 D_80094784;
extern u8 D_8011FDC8;

extern void func_80078970();
extern void func_800469F4();
extern void func_80011DFC();

void func_8014F910(void)
{
    u32 sel;

    D_8009473C = D_80124250;
    D_80094740 = D_801242BC;
    D_80094744 = D_80124328;
    sel = D_800B0895;

    if (sel < 98) {
        if (sel != 97) goto L94C;
        if (D_800B0896 < 3U) goto L94C;
    }
    D_80125304 = 1;
    goto L954;
L94C:
    D_80125304 = 0;
L954:
    if (sel == 98) goto L97C;
    if (sel != 97) goto L98C;
    if (D_800B0896 < 12U) goto L98C;
L97C:
    D_80125308 = 1;
    goto L994;
L98C:
    D_80125308 = 0;
L994:
    D_801252E8 = 0;
    func_80078970(&D_80094784, &D_8011FDC8);
    func_800469F4(17051);
    func_80011DFC();
}
