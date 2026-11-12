/* @SymbolStart: 0x80144A14 */
/* @SymbolSize: 108 */
/* @PostProc: patch_insn_reg_field 0x18:rt:v0:v1 0x1c:rs:v0:v1 0x1c:rt:v0:v1 0x30:rs:v0:v1 0x3c:rs:v0:v1 */
/* Address: 0x80144A14 (cross-prologue head) / 0x80144A24 (prologue) */
#include "types.h"

extern void name_ent_intro_step(void);
extern void name_ent_movie_exit(void);
extern s32 step_state;
extern u8 now_sub_step;

void name_ent_intro_main(void)
{
    s32 val;
    s32 sel;

    val = step_state;
    sel = now_sub_step;

    step_state = val + 1;

    switch (sel) {
    case 0:
        name_ent_intro_step();
        break;
    case 1:
        name_ent_movie_exit();
        break;
    }
}
