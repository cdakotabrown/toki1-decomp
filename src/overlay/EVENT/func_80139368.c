/* @SymbolStart: 0x80139360 */
/* @SymbolSize: 124 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x20:rs:v0:v1 0x2c:rs:v0:v1 0x34:rs:v0:v1 */
/* Address: 0x80139368 */
/* Size: 124 bytes (31 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECB0;

extern void func_800FD3DC();
extern void func_800FD43C();
extern void func_800FD49C();
extern void func_80015FE0();

void func_80139368(void) {
    s32 v1_arg_v = D_800EECB0;

    switch (v1_arg_v) {
        case 1:
            func_800FD3DC();
            break;
        case 2:
            func_800FD43C();
            break;
        case 3:
            func_800FD49C();
            break;
        default:
            func_80015FE0();
            break;
    }
}
