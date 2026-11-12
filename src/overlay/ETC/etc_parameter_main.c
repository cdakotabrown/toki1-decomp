/* @SymbolStart: 0x80148064 */
/* @SymbolSize: 88 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 */
/* Address: 0x8014806C */
/* Size: 80 bytes (20 instructions) */
#include "types.h"

extern void parameter_show(void);
extern void etc_parameter_init(void);
extern void etc_parameter_exit(void);

extern u8 now_sub_sub_step;

void etc_parameter_main(void)
{
    s32 v1 = now_sub_sub_step;

    switch (v1) {
    case 0:
        etc_parameter_init();
        break;
    case 1:
        etc_parameter_exit();
        break;
    }
    parameter_show();
}
