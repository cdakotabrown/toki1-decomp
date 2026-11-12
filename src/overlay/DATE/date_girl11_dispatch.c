/* @SymbolStart: 0x8015A3A0 */
/* @SymbolSize: 100 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x20:rs:v0:v1 0x2c:rs:v0:v1 */
/* Function: date_girl11_dispatch */
/* Overlay: DATE */
/* Address: 0x8015A3A0 (cross-prologue head) / 0x8015A3A8 (body) */
/* Size: 100 bytes (25 instructions) */
#include "types.h"

extern s32 work_tarao;

extern void game_reset(void);
extern void date_girl11_main3(void);
extern void date_girl11_main5(void);

void date_girl11_dispatch(void) {
    switch (work_tarao) {
        case 3:
            date_girl11_main3();
            return;
        case 5:
            date_girl11_main5();
            return;
        default:
            game_reset();
            return;
    }
}
