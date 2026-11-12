/* @SymbolStart: 0x8013AF80 */
/* @SymbolSize: 100 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x20:rs:v0:v1 0x2c:rs:v0:v1 */
/* geko_girl2_main -- GEKO overlay, Address: 0x8013AF88, Size: 92 bytes (23 insns) */
#include "types.h"

extern void geko_girl2_ev1_main(void);
extern void geko_girl2_ev7_main(void);
extern void game_reset(void);
extern s32 work_tarao;

void geko_girl2_main(void)
{
    s32 v1 = work_tarao;

    switch (v1) {
    case 1:
        geko_girl2_ev1_main();
        break;
    case 7:
        geko_girl2_ev7_main();
        break;
    default:
        game_reset();
        break;
    }
}
