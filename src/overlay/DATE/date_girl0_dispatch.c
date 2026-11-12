/* @SymbolStart: 0x8014F360 */
/* @SymbolSize: 148 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x20:rs:v0:v1 0x2c:rs:v0:v1 0x34:rs:v0:v1 0x3c:rs:v0:v1 */
/* Function: date_girl0_dispatch */
/* Overlay: DATE */
/* Address: 0x8014F360 (cross-prologue head) / 0x8014F368 (body) */
/* Size: 148 bytes (37 instructions) */
#include "types.h"

extern s32 work_tarao;

extern s32 game_reset(void);
extern s32 date_fntab_dispatch3(void);
extern s32 date_fntab_dispatch4(void);
extern s32 date_fntab_dispatch5(void);
extern s32 date_fntab_dispatch6(void);

s32 date_girl0_dispatch(void)
{
    switch (work_tarao) {
    case 3:
        return date_fntab_dispatch3();
    case 4:
        return date_fntab_dispatch4();
    case 5:
        return date_fntab_dispatch5();
    case 6:
        return date_fntab_dispatch6();
    default:
        return game_reset();
    }
}
