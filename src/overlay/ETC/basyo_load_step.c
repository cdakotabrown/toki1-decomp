/* @SymbolStart: 0x80132460 */
/* @SymbolSize: 80 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 */
/* Address: 0x80132468 */
/* Size: 80 bytes (20 instructions) */
#include "types.h"

extern void pad_wait_sub_step();
extern void basyo_load_init();
extern u8 now_sub_sub_step;

void basyo_load_step(void)
{
    s32 v1 = now_sub_sub_step;

    switch (v1) {
    case 0:
        basyo_load_init();
        break;
    case 1:
        pad_wait_sub_step();
        break;
    }
}
