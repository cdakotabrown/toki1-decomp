/* @SymbolStart: 0x80155200 */
/* @SymbolSize: 92 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x1c:rt:v0:v1 0x20:rt:v0:v1 0x28:rs:v0:v1 */
/* Function: date_load3d_wait2 */
/* Overlay: DATE */
/* Address: 0x80155200 (cross-prologue head) / 0x80155214 (body) */
/* Size: 92 bytes (23 instructions) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern void cd_read_start2(s32, s32, s32);
extern void date_bufaddr_init(void);

extern s32 step_counter;

s32 date_load3d_wait2(void)
{
    s32 v;

    v = step_counter + 1;
    step_counter = v;
    if ((u32)v < 16) {
        return 0;
    }
    cd_read_start2(61, 0x801B0000, 0x872B);
    date_bufaddr_init();
    return sub_sub_step_up();
}
