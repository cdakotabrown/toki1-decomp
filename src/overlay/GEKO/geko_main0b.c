/* @SymbolStart: 0x80140BC0 */
/* @SymbolSize: 124 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x20:rs:v0:v1 0x2c:rs:v0:v1 0x34:rs:v0:v1 */
/* geko_main0b -- GEKO overlay, Address: 0x80140BC8, Size: 116 bytes (29 insns) */
#include "types.h"

extern void geko_girl11_ev1_main(void);
extern void geko_main2b(void);
extern void geko_girl11_ev4_main(void);
extern void game_reset(void);
extern s32 work_tarao;

void geko_main0b(void)
{
    s32 v1 = work_tarao;

    switch (v1) {
    case 1:
        geko_girl11_ev1_main();
        break;
    case 2:
        geko_main2b();
        break;
    case 4:
        geko_girl11_ev4_main();
        break;
    default:
        game_reset();
        break;
    }
}
