/* @SymbolStart: 0x8013F6E8 */
/* @SymbolSize: 156 */
/* @PostProc: patch_insn_reg_field 0x20:rt:v0:a2 0x28:rs:v0:a2 0x34:rs:v0:a2 0x50:rt:v0:a2 0x58:rs:v0:a2 0x60:rs:v0:a2 0x7c:rt:v0:a2 0x84:rs:v0:a2 0x8c:rs:v0:a2 */
#include "types.h"

extern s16 *taco_actor_rec;

extern void taco_icon_disp(s32 a0, s32 a1);

void taco_status_disp(void)
{
    s16 *rec = taco_actor_rec;

    s32 v;

    v = rec[62];
    if (v == 3 || v == 4) {
        taco_icon_disp(180, 96);
    }

    v = taco_actor_rec[63];
    if (v == 3 || v == 4) {
        taco_icon_disp(200, 96);
    }

    v = taco_actor_rec[64];
    if (v == 3 || v == 4) {
        taco_icon_disp(220, 96);
    }
}
