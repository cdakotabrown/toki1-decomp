/* @SymbolStart: 0x80154FC8 */
/* @SymbolSize: 64 */
/* Address: 0x80154FD8 (true start 0x80154FC8) */
/* Size: 64 bytes (16 instructions, head + body) */
#include "types.h"

extern s32 D_800EECBC;
extern s16 D_80124394[];

extern void func_8004A8EC();
extern void func_80011DFC();

void func_80154FD8(void) {
    func_8004A8EC(D_80124394[D_800EECBC]);
    func_80011DFC();
}
