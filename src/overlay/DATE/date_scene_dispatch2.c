/* @SymbolStart: 0x801502A4 */
/* @SymbolSize: 148 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x20:rs:v0:v1 0x2c:rs:v0:v1 0x34:rs:v0:v1 0x3c:rs:v0:v1 */
/* Function: date_scene_dispatch2 */
/* Overlay: DATE */
/* Address: 0x801502A4 (cross-prologue head) / 0x801502AC (body) */
/* Size: 148 bytes (37 instructions) */
#include "types.h"

extern s32 work_tarao;

extern s32 game_reset(void);
extern s32 date_girl2_mode2_dispatch(void);
extern s32 date_girl2_mode3_dispatch(void);
extern s32 date_girl2_mode4_dispatch(void);
extern s32 date_girl2_mode5_dispatch(void);

s32 date_scene_dispatch2(void)
{
    switch (work_tarao) {
    case 2:
        return date_girl2_mode2_dispatch();
    case 3:
        return date_girl2_mode3_dispatch();
    case 4:
        return date_girl2_mode4_dispatch();
    case 5:
        return date_girl2_mode5_dispatch();
    default:
        return game_reset();
    }
}
