/* @SymbolStart: 0x80154550 */
/* @SymbolSize: 148 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x20:rs:v0:v1 0x2c:rs:v0:v1 0x34:rs:v0:v1 0x3c:rs:v0:v1 */
/* Function: date_scene_dispatch3 */
/* Overlay: DATE */
/* Address: 0x80154550 (cross-prologue head) / 0x80154558 (body) */
/* Size: 148 bytes (37 instructions) */
#include "types.h"

extern s32 work_tarao;

extern void game_reset(void);
extern void date_girl1_mode2_dispatch(void);
extern void date_girl1_mode3_dispatch(void);
extern void date_girl1_mode4_dispatch(void);
extern void date_girl1_mode5_dispatch(void);

void date_scene_dispatch3(void) {
    switch (work_tarao) {
        case 2:
            date_girl1_mode2_dispatch();
            return;
        case 3:
            date_girl1_mode3_dispatch();
            return;
        case 4:
            date_girl1_mode4_dispatch();
            return;
        case 5:
            date_girl1_mode5_dispatch();
            return;
        default:
            game_reset();
            return;
    }
}
