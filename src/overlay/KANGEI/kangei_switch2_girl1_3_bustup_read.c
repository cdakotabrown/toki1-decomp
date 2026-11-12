/* @SymbolSize: 120 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x20:rs:v0:v1 0x2c:rs:v0:v1 */
#include "types.h"

extern void bustup_cd_read(s32);
extern void sub_sub_step_up(void);
extern u8 now_girl;

void kangei_switch2_girl1_3_bustup_read(void)
{
    s32 v1 = now_girl;

    switch (v1) {
    case 1:
        bustup_cd_read(24368);
        break;
    case 3:
        bustup_cd_read(23198);
        break;
    default:
        sub_sub_step_up();
        sub_sub_step_up();
        break;
    }
    sub_sub_step_up();
}
