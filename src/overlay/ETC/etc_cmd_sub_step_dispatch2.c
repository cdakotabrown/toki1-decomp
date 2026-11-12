/* @SymbolStart: 0x801326FC */
/* @SymbolSize: 96 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 */
/* Address: 0x80132704 */
/* Size: 96 bytes (24 instructions) */
#include "types.h"

extern void basyo_show(void);
extern void update_anim_state(s32);
extern void etc_cmd_dispatch3(void);
extern void etc_cmd_dispatch4(void);
extern u8 now_sub_step;

void etc_cmd_sub_step_dispatch2(void)
{
    s32 v1 = now_sub_step;

    switch (v1) {
    case 0:
        etc_cmd_dispatch3();
        break;
    case 1:
        etc_cmd_dispatch4();
        break;
    }
    basyo_show();
    update_anim_state(0);
}
