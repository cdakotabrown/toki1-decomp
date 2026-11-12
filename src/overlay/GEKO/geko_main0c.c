/* @SymbolStart: 0x8013B940 */
/* @SymbolSize: 124 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x20:rs:v0:v1 0x2c:rs:v0:v1 0x34:rs:v0:v1 */
/* geko_main0c -- GEKO overlay, Address: 0x8013B948, Size: 116 bytes (29 insns) */
#include "types.h"

extern void geko_girl12_ev1_main(void);
extern void geko_girl12_ev2_main(void);
extern void geko_girl12_ev3_main(void);
extern void game_reset(void);
extern s32 work_tarao;

void geko_main0c(void)
{
    s32 v1 = work_tarao;

    switch (v1) {
    case 1:
        geko_girl12_ev1_main();
        break;
    case 2:
        geko_girl12_ev2_main();
        break;
    case 3:
        geko_girl12_ev3_main();
        break;
    default:
        game_reset();
        break;
    }
}
