/* Address: 0x801330F0 (absorbed head)  Size: 108 bytes (27 instructions) */
/* @SymbolStart: 0x801330F0 */
/* @SymbolSize: 108 */
#include "types.h"

extern s32 D_800EECB4;
extern s32 D_8007E7D0[];
extern s32 D_8007E810[];
extern s32 D_8007E850[];

extern void func_800143DC();
extern void func_80011DFC(void);

void func_80133118(void) {
    s32 i;

    i = D_800EECB4;
    func_800143DC(1, 0xB19B, 0xB178, D_8007E7D0[i], D_8007E810[i],
                  D_8007E850[i]);
    func_80011DFC();
}
