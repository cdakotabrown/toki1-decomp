/* @SymbolStart: 0x80154F40 */
/* @SymbolSize: 64 */
/* Address: 0x80154F50 (true start 0x80154F40) */
/* Size: 64 bytes (16 instructions, head + body) */
#include "types.h"

extern s32 D_800EECBC;
extern s16 D_8012438C[];

extern void func_8004A8EC();
extern void func_80011DFC();

void func_80154F50(void) {
    func_8004A8EC(D_8012438C[D_800EECBC]);
    func_80011DFC();
}
