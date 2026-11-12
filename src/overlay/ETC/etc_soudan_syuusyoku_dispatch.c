/* @SymbolStart: 0x8014C1D0 */
/* @SymbolSize: 84 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-8 IDO52_UGEN_DEST_TEMP=1 */
/* @PostProc: patch_insn_reg_field 0x10:rt:t6:v1 0x14:rs:t6:v1 0x14:rt:t6:v1 0x20:rs:t6:v1 0x24:rs:t6:v1 0x2c:rs:t6:v1 */
/* Address: 0x8014C1D0 (cross-prologue head: lbu v1, now_sub_sub_step) / */
#include "types.h"

extern u8 now_sub_sub_step;
extern void etc_soudan_bg_load(void);
extern void etc_syuusyoku_step(void);

s32 etc_soudan_syuusyoku_dispatch(void)
{
    u32 sel;

    sel = now_sub_sub_step;
    switch (sel) {
    case 0:
        etc_soudan_bg_load();
        break;
    case 1:
        etc_syuusyoku_step();
        break;
    }
}
