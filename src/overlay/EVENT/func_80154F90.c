/* @SymbolStart: 0x80154F80 */
/* @SymbolSize: 72 */
/* Address: 0x80154F90 (true start 0x80154F80) */
/* Size: 72 bytes (18 instructions, head + body) */
#include "types.h"

extern s32 D_800EECBC;
extern s16 D_80124394[];

extern void func_8004A8EC();
extern void func_80033E88();
extern void func_80011DFC();

void func_80154F90(void) {
    func_8004A8EC(D_80124394[D_800EECBC]);
    func_80033E88();
    func_80011DFC();
}
