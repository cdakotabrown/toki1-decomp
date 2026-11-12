/* @SymbolStart: 0x801513C0 */
/* @SymbolSize: 136 */
/* Address: 0x801513C8, Size: 128 bytes (32 instructions) */
#include "types.h"

extern u8 D_801252F4;

extern void func_800469F4(s32);
extern void func_80011DFC(void);
extern u8 D_801252F8;
extern u8 D_801252FC;

void func_801513C8(void) {
    u8 t6_hidden_v = D_801252F4;

    if (t6_hidden_v != 0) {
        func_800469F4(17519);
    } else if (D_801252F8 != 0) {
        func_800469F4(17529);
    } else if (D_801252FC != 0) {
        func_800469F4(17538);
    } else {
        func_800469F4(17510);
    }
    func_80011DFC();
}
