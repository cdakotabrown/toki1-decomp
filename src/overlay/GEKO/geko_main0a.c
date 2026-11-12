/* @SymbolStart: 0x8013F420 */
/* @SymbolSize: 124 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x20:rs:v0:v1 0x2c:rs:v0:v1 0x34:rs:v0:v1 */
/* geko_main0a -- GEKO overlay, Address: 0x8013F428, Size: 116 bytes (29 insns) */
#include "types.h"

extern void geko_scene67_guard(void);
extern void geko_scene68_guard(void);
extern void geko_sub_step_guard(void);
extern void game_reset(void);
extern s32 work_tarao;

void geko_main0a(void)
{
    s32 v1 = work_tarao;

    switch (v1) {
    case 1:
        geko_scene67_guard();
        break;
    case 2:
        geko_scene68_guard();
        break;
    case 7:
        geko_sub_step_guard();
        break;
    default:
        game_reset();
        break;
    }
}
