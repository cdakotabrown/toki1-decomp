/* @SymbolStart: 0x801323D0 */
/* @SymbolSize: 144 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 0x38:rs:v0:v1 */
/* Address: 0x801323D8 */
/* Size: 144 bytes (36 instructions) */
#include "types.h"

extern void basyo_show(void);
extern void update_anim_state(s32);
extern void basyo_load_step(void);
extern void etc_basyo_init_dispatch(void);
extern void etc_cmd_dispatch1(void);
extern void etc_cmd_dispatch2(void);
extern u8 now_sub_step;

void etc_cmd_sub_step_dispatch(void)
{
    s32 v1 = now_sub_step;

    switch (v1) {
    case 0:
        basyo_load_step();
        break;
    case 1:
        etc_basyo_init_dispatch();
        break;
    case 2:
        etc_cmd_dispatch1();
        break;
    case 3:
        etc_cmd_dispatch2();
        break;
    }
    update_anim_state(0);
    basyo_show();
}
