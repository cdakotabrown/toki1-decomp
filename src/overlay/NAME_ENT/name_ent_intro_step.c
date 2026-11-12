/* @SymbolStart: 0x80144978 */
/* @SymbolSize: 156 */
/* @PostProc: patch_insn_reg_field 0x18:rt:v0:v1 0x1c:rs:v0:v1 0x1c:rt:v0:v1 0x30:rs:v0:v1 0x3c:rs:v0:v1 0x44:rs:v0:v1 */
/* Address: 0x80144978 (cross-prologue head) / 0x80144988 (prologue) */
/* Size: 156 bytes (39 insns) */
#include "types.h"

extern void name_ent_load(void);
extern void name_ent_intro(void);
extern void name_ent_intro_exit(void);
extern void goto_sub_sub_step(s32 a0);
extern s32 step_counter;
extern u8 now_sub_sub_step;
extern u32 pad_trig;

void name_ent_intro_step(void) {
    s32 val;
    s32 cmd;

    val = step_counter;
    cmd = now_sub_sub_step;

    step_counter = val + 1;
    switch (cmd) {
    case 0:
        name_ent_load();
        break;
    case 1:
        name_ent_intro();
        break;
    case 2:
        name_ent_intro_exit();
        break;
    }
    if (pad_trig & 0x860) {
        goto_sub_sub_step(2);
    }
}
