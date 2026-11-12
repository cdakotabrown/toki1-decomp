/* Function: DrawPrim */
/* Address: 0x800A041C */
/* Size: 100 bytes (25 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: patch_stack_complete --old 32 --new 40 */
/* @PostProc: remap_stack_slots --keep-frame --map 32:40,28:36,20:28 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field --patch 0x28:rt:t9:t1 --patch 0x30:rs:t9:t1 --patch 0x38:rt:t1:t2 --patch 0x3c:rt:t1:t2 --patch 0x3c:rs:t1:t2 --patch 0x44:rt:t9:t3 --patch 0x44:rs:t1:t2 --patch 0x4c:rs:t9:t3 */
#include "types.h"

extern u32 *graph_jtb;

void DrawPrim(u32 *volatile a0) {
    volatile u32 byte;
    u32 *table;

    byte = (*(volatile u32 *)a0 >> 24) & 0xFF;

    table = graph_jtb;
    ((void (*)(s32))table[15])(0);

    table = graph_jtb;
    ((void (*)(u32 *, u32))table[5])((u32 *)((u8 *)a0 + 4), byte);
}
