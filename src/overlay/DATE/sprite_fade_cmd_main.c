/* @SymbolStart: 0x80135C5C */
/* @SymbolSize: 72 */
/* @PostProc: patch_insn_reg_field 0x2c:rt:v0:t7 0x30:rs:v0:t7 0x30:rt:v0:t7 0x38:rs:v0:t7 0x38:rt:t7:t8 0x3c:rt:t7:t8 */
/* Address: 0x80135C5C (cross-prologue head) / 0x80135C64 (body) */
/* Size: 72 bytes (18 instructions) */
#include "types.h"

extern volatile u8 step_mode;

extern void device_cmd_send(s32 arg);
extern void sprite_fade_main(void);

void sprite_fade_cmd_main(void) {
    if (step_mode == 0) {
        device_cmd_send(514);
        step_mode++;
    }
    sprite_fade_main();
}
