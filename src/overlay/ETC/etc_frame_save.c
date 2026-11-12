/* @PostProc: patch_stack_complete --old 40 --new 56 */
/* @PostProc: swap_registers t6:t7 */
/* @PostProc: swap_registers t8:t7 */
/* @PostProc: swap_registers t9:t8 */
/* @PostProc: swap_registers t0:t9 */
/* @PostProc: remap_stack_slots --from 32 --to 44 --from 34 --to 46 --from 36 --to 48 --from 38 --to 50 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:t6 0x14:rs:v0:t6 0x14:rt:v0:t6 0x1c:rt:v0:t6 0x20:rt:v0:t6 */
/* Address: 0x8013D25C */
#include "types.h"

extern void DrawSync(s32);
extern void MoveImage(RECT*, s32, s32);
extern s32 activeBuff;

void etc_frame_save(void)
{
    RECT rect;
    s32 yPos;

    yPos = ((activeBuff << 4) - activeBuff) << 4;
    rect.x = 0;
    rect.y = yPos;
    rect.w = 320;
    rect.h = 240;
    MoveImage(&rect, 320, 256);
    DrawSync(0);
}
