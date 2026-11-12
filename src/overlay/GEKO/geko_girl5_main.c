/* @SymbolStart: 0x8013D4B0 */
/* @SymbolSize: 100 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x20:rs:v0:v1 0x2c:rs:v0:v1 */
/* geko_girl5_main -- GEKO overlay, Address: 0x8013D4B8, Size: 92 bytes (23 insns) */
#include "types.h"

extern void geko_sub_sub_guard2(void);
extern void geko_sub_sub_guard3(void);
extern void game_reset(void);
extern s32 work_tarao;

void geko_girl5_main(void)
{
    s32 v1 = work_tarao;

    switch (v1) {
    case 1:
        geko_sub_sub_guard2();
        break;
    case 2:
        geko_sub_sub_guard3();
        break;
    default:
        game_reset();
        break;
    }
}
