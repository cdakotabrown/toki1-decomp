/* @SymbolStart: 0x80132134 */
/* @PostProc: patch_insn_reg_field 0x18:rt:v0:v1 0x1c:rs:v0:v1 0x1c:rt:v0:v1 0x30:rs:v0:v1 0x3c:rs:v0:v1 */
/* @SymbolSize: 100 */
/* Address: 0x80132134 (cross-prologue head) / 0x80132144 (prologue) */
/* Size: 100 bytes (25 insns) */
#include "types.h"

extern s32 step_counter;
extern u8 now_sub_sub_step;

extern void name_ent_cd_load(void);
extern void name_ent_fade_in(void);

void name_ent_input_main0(void)
{
    s32 val;
    s32 sel;

    val = step_counter;
    sel = now_sub_sub_step;

    step_counter = val + 1;

    switch (sel) {
    case 0:
        name_ent_cd_load();
        break;
    case 1:
        name_ent_fade_in();
        break;
    }
}
