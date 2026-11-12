/* @SymbolStart: 0x80151B28 */
/* @SymbolSize: 96 */
/* Address: 0x80151B30 */
/* Size: 96 bytes (24 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B1746;

extern s16 D_800B095E;

extern void func_800469F4(s32 sound_id);
extern void func_80011DFC(void);

void func_80151B30(void) {
    u8 t6_v = D_800B1746;

    if (t6_v != 6 || D_800B095E < 150) {
        func_800469F4(17688);
    } else {
        func_800469F4(17698);
    }
    func_80011DFC();
}
