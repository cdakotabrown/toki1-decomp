/* Address: 0x800A0CCC */
/* Size: 92 bytes (23 instructions) */
/* Region: IDO (0x80086000+) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_registers a2 v1 */
/* @PostProc: swap_registers a3 a2 */
/* @PostProc: patch_insn_reg_field --patch 0x0c:rt:v0:t6 --patch 0x18:rs:v0:t6 --patch 0x18:rd:t6:t7 --patch 0x20:rs:t6:t7 --patch 0x20:rd:t7:t8 --patch 0x28:rt:t7:t8 */
/* (2026-08-29: a dead `@PostProc: strip_instructions` was removed here. */
/* Function: Modifies structure header and calls get_ofs */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
} Pos;

extern s32 get_ofs(s16 x, s16 y);

void SetDrawOffset(u32 *ptr, Pos *pos) {
    volatile u32 *p = ptr;
    volatile Pos *q = pos;
    u32 val;
    u32 masked;
    u32 combined;
    s16 y;
    s16 x;

    val = *p;
    masked = val & 0x00FFFFFF;
    combined = masked | 0x02000000;
    *p = combined;
    y = q->y;
    x = q->x;
    ((u32*)p)[1] = get_ofs(x, y);
    ((u32*)p)[2] = 0;
}
