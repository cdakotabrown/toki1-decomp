/* Address: 0x80087220 */
/* Size: 128 bytes (32 instructions) */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80081000-0x800BFFFF) */
#include "types.h"

extern void func_80086A50(u32* ptr, u32 val);
extern void func_80086A80(u32* ptr, u32 val);
extern void MDEC_in(u32* ptr, u16 val);

void func_80087220(u32 *arg0, s32 flags) {
    u32 val;

    if (flags & 1) {

        val = *arg0;
        val = val & 0xF7FFFFFF;

        func_80086A50(arg0, val);
        return;
    } else {

        val = *arg0;
        val = val | 0x08000000;
        *arg0 = val;
    }

    if (flags & 2) {

        val = *arg0;
        val = val | 0x02000000;
        func_80086A80(arg0, val);
        return;
    } else {

        val = *arg0;
        val = val & 0xFDFFFFFF;
        *arg0 = val;
    }

    MDEC_in(arg0, (u16)*arg0);
}
