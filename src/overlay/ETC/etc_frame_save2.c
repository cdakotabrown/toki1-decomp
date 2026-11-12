/* @SymbolStart: 0x8013FB88 */
/* @PostProc: patch_stack_complete --old 32 --new 56 */
/* @SymbolSize: 92 */
/* @PostProc: rename_register_chain --chain a0:t6:t7:t8:t9 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* @PostProc: remap_stack_slots --from 26 --to 46 */
/* @PostProc: remap_stack_slots --from 28 --to 48 */
/* @PostProc: remap_stack_slots --from 30 --to 50 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t6:a0 0x10:rt:t7:t6 0x14:rs:t7:t6 0x14:rt:t7:t6 0x1c:rd:t8:t7 0x1c:rt:t7:t6 0x20:rd:t8:t7 0x20:rs:t8:t7 0x20:rt:t7:t6 0x28:rd:t8:t7 0x28:rt:t8:t7 0x2c:rt:t9:t8 0x38:rt:t8:t7 0x3c:rt:t9:t8 */
/* @PostProc: patch_insn_reg_field 0x58:rd:t6:a0 */
/* Address: 0x8013FB90 */
/* Size: 84 bytes (21 instructions) */
#include "types.h"

extern void DrawSync(s32);
extern void MoveImage(void *, s32, s32);
extern s32 activeBuff;

typedef struct {
    s16 a;
    s16 b;
    s16 c;
    s16 d;
} Quad16;

void etc_frame_save2(void)
{
    Quad16 q;

    q.a = 0;
    q.b = (s16)(activeBuff * 240);
    q.c = 320;
    q.d = 240;
    MoveImage(&q, 640, 0);
    DrawSync(0);
}
