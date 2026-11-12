/* Address: 0x8013D2B0 (absorbed head)  Size: 108 bytes (27 instructions) */
/* @SymbolStart: 0x8013D2B0 */
/* @SymbolSize: 108 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=240 */
/* @PostProc: patch_stack_complete --old 40 --new 56 */
/* @PostProc: remap_stack_slots --from 32 --to 44 --from 34 --to 46 --from 36 --to 48 --from 38 --to 50 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rs:v1:v0 0x14:rt:v1:v0 0x18:rt:t7:t6 0x1c:rt:v0:v1 0x20:rd:t8:t7 0x20:rs:t7:t6 0x20:rt:v1:v0 0x24:rs:t8:t7 0x24:rt:v0:v1 0x30:rt:t6:t9 0x38:rt:t6:t9 0x3c:rt:v0:v1 0x48:rd:t9:t8 0x4c:rt:t9:t8 0x54:rs:v1:v0 0x54:rt:v0:v1 */
#include "types.h"

extern s32 activeBuff;

extern void MoveImage();
extern void DrawSync();

void etc_frame_copy(void) {
    s16 rect[4];
    s32 page;

    page = activeBuff;
    rect[0] = 0;
    rect[2] = 320;
    rect[3] = 240;
    rect[1] = (s16)((1 - page) * 240);
    MoveImage(rect, 0, page * 240);
    DrawSync(0);
}
