/* Address: 0x80133AD0 (absorbed head)  Size: 108 bytes (27 instructions) */
/* @SymbolStart: 0x80133AD0 */
/* @SymbolSize: 108 */
/* @PostProc: patch_stack_complete --old 40 --new 56 */
/* @PostProc: remap_stack_slots --map '24:44,32:52' */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:t6 0x14:rs:v0:t6 0x14:rt:t7:t0 0x18:rt:t6:t7 0x1c:rd:t8:t1 0x1c:rt:t7:t0 0x20:rs:v0:t6 0x24:rs:t6:t7 0x24:rt:t6:t7 0x28:rd:v1:v0 0x28:rs:t8:t9 0x28:rt:t9:t1 0x30:rs:v1:v0 0x30:rt:t0:t2 0x38:rd:t1:t3 0x38:rs:t0:t7 0x38:rt:t6:t2 0x3c:rs:t1:t3 0x3c:rt:t2:t4 0x40:rs:v0:t6 0x44:rs:t2:t4 0x4c:rt:t0:t2 0x54:rt:t6:t7 0x58:rt:v1:v0 0x5c:rt:a2:v1 0x60:rt:t3:t5 0x64:rd:t4:t6 0x64:rs:a2:v1 0x64:rt:v1:v0 0x68:rs:t4:t6 0x68:rt:t3:t5 */
#include "types.h"

extern u8 *tt_game_wk;

extern void device_cmd_send();

void tt_cmd_once(s32 arg) {
    u16 code;
    u16 idx;
    u8 *base;

    code = (u16)arg;
    idx = (u16)((((u32)code & 0x100) >> 3) | (code & 0x1F));
    base = tt_game_wk;

    if (base[idx + 0x56] == 0) {
        device_cmd_send(code);
        base[idx + 0x56] = 1;
    }
}
